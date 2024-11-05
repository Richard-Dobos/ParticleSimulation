#pragma once

#include <iostream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <memory>
#include <sstream>

#ifndef ECS_ASSERTS
#define ECS_ASSERT(condition, msg) \
		if (!(condition)) { \
			std::cerr << "[ECS error]: " << msg << std::endl; \
			::abort(); \
		}

#define ENTITY_INFO(id) \
			"['" << GetEntityName(id) << "', ID: " << id << "]"

#define ECS_ASSERT_VALID_ENTITY(id) \
			ECS_ASSERT(id != NULL_ENTITY, "NULL_ENTITY cannot be operated on by the ECS") \
			ECS_ASSERT(id < m_maxEntityID && id >= 0, "Invalid entity ID out of bounds: " << id);

#define ECS_ASSERT_ALIVE_ENTITY(id) \
			ECS_ASSERT(m_entityMasks.Get(id) != nullptr, "Attempting to access inactive entity with ID: " << id);
#endif

#ifndef ECS_INFO
#ifdef ECS_INFO_ENABLED
#define ECS_INFO(msg) std::cout << "[ECS info]: " << msg << "\n";
#else
#define ECS_INFO(msg);
#endif
#endif


namespace Core::Engine 
{
	using EntityID = uint64_t;

	static constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();
	constexpr size_t MAX_ENTITIES = NULL_ENTITY;
	constexpr size_t MAX_COMPONENTS = 64;

	using ComponentMask = std::bitset<MAX_COMPONENTS>;
	using TypeName = const char*;

	class ISparseSet
	{
	public:
		virtual ~ISparseSet() = default;

		virtual void Delete(EntityID) = 0;
		virtual void Clear() = 0;
		virtual size_t Size() = 0;
	};

	template <typename T>
	class SparseSet : public ISparseSet 
	{
	public:
		SparseSet() { m_dense.reserve(200); }
		~SparseSet() = default;

		T* Set(EntityID id, T obj) 
		{
			size_t index = GetDenseIndex(id);
			
			if (index != tombstone) 
			{
				m_dense[index] = obj;
				m_denseToEntity[index] = id;

				return &m_dense[index];
			}

			SetDenseIndex(id, m_dense.size());

			m_dense.push_back(obj);
			m_denseToEntity.push_back(id);

			return &m_dense.back();
		}

		T* Get(EntityID id) 
		{
			size_t index = GetDenseIndex(id);
			return (index != tombstone) ? &m_dense[index] : nullptr;
		}

		void Delete(EntityID id) override 
		{
			size_t deletedIndex = GetDenseIndex(id);

			if (m_dense.empty() || deletedIndex == tombstone) return;

			SetDenseIndex(m_denseToEntity.back(), deletedIndex);
			SetDenseIndex(id, tombstone);

			std::swap(m_dense.back(), m_dense[deletedIndex]);
			std::swap(m_denseToEntity.back(), m_denseToEntity[deletedIndex]);

			m_dense.pop_back();
			m_denseToEntity.pop_back();
		}

		size_t Size() override 
		{
			return m_dense.size();
		}

		void Clear() override 
		{
			m_dense.clear();
			m_sparsePages.clear();
			m_denseToEntity.clear();
		}

		bool IsEmpty() const 
		{
			return m_dense.empty();
		}

		const std::vector<T>& Data() const 
		{
			return m_dense;
		}

		void PrintDense() 
		{
			std::stringstream ss;
			std::string delim = "";
			for (const T& e : m_dense) {
				ss << delim << e;
				if (delim.empty())
					delim = ", ";
			}
			ECS_INFO("[" << ss.str() << "]");
		}

	private:
		void SetDenseIndex(EntityID id, size_t index) 
		{
			size_t page = id / SPARSE_MAX_SIZE;
			size_t sparseIndex = id % SPARSE_MAX_SIZE; // Index local to a page

			if (page >= m_sparsePages.size())
				m_sparsePages.resize(page + 1);

			Sparse& sparse = m_sparsePages[page];
			if (sparseIndex >= sparse.size())
				sparse.resize(sparseIndex + 1, tombstone);

			sparse[sparseIndex] = index;
		}

		size_t GetDenseIndex(EntityID id) {
			size_t page = id / SPARSE_MAX_SIZE;
			size_t sparseIndex = id % SPARSE_MAX_SIZE;

			if (page < m_sparsePages.size()) 
			{
				Sparse& sparse = m_sparsePages[page];

				if (sparseIndex < sparse.size())
					return sparse[sparseIndex];
			}

			return tombstone;
		}

	private:
		using Sparse = std::vector<size_t>;

		std::vector<T> m_dense;
		std::vector<Sparse> m_sparsePages;
		std::vector<EntityID> m_denseToEntity; // 1:1 vector where dense index == Entity Index

		const size_t SPARSE_MAX_SIZE = 1000;
		static constexpr size_t tombstone = std::numeric_limits<size_t>::max();
	};

	class ECS 
	{
	public:
		ECS() = default;
		~ECS()
		{
			delete m_componentPools;
		}

		/*
		*  Creates an entity and returns the ID to refer to that entity.
		*
		*  @param(name):
		*  * Optional and used for debugging purposes, it
		*    shouldn't be used often since there's no optimization
		*/

		EntityID CreateEntity(std::string_view name = "") 
		{
			EntityID id = tombstone;

			if (m_AvailableEntities.size() == 0) 
			{
				ECS_ASSERT(m_maxEntityID < MAX_ENTITIES, "Entity limit exceeded");
				id = m_maxEntityID++;
			}

			else
			{
				id = m_AvailableEntities.back();
				m_AvailableEntities.pop_back();
			}

			ECS_ASSERT(id != tombstone, "Cannot create entity with null ID");

			m_entityMasks.Set(id, {});

			if (!name.empty())
			{
				m_entityNames[id] = name;
			}

			ECS_INFO("Created entity " << ENTITY_INFO(id));

			return id;
		}

		std::string GetEntityName(EntityID id) 
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			auto it = m_entityNames.find(id);
			if (it == m_entityNames.end())
				return "Entity";

			return it->second;
		}

		/*
		* Deletes an active entity and its associated components.
		* - Overwrites the given entity to NULL_ENTITY.
		*
		* This should NOT be used in the middle of a system while iterating
		* through entities, as it will remove from the list immediately. Use
		* FlagEntity(id, true) to mark an entity for deletion, and then DeleteFlagged()
		* At the end of a frame to clear all flagged entities instead.
		*/
		void DeleteEntity(EntityID& id) 
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			std::string name = GetEntityName(id);
			ComponentMask& mask = GetEntityMask(id);

			// Remove from group
			RemoveFromGroup(mask, id);

			// Destroy component associations
			for (int i = 0; i < MAX_COMPONENTS; i++)
			{
				if (mask[i] == 1)
				{
					(*m_componentPools)[i]->Delete(id);
				}
			}

			m_entityMasks.Delete(id);
			m_entityNames.erase(id);
			m_AvailableEntities.push_back(id);

			ECS_INFO("Deleted entity ['" << name << "', ID: " << id << "]");
			id = NULL_ENTITY;
		}

		/*
		*  Register a component and create a pool for it
		*/
		template <typename T>
		void RegisterComponent() 
		{
			TypeName name = typeid(T).name();

			ECS_ASSERT(m_componentBitPosition.find(name) == m_componentBitPosition.end(),
						"Component with name '" << name << "' already registered");
			ECS_ASSERT(m_componentPools->size() < MAX_COMPONENTS,
						"Exceeded max number of registered components");

			//m_componentBitPosition.emplace(name, m_componentPools.size());
			m_componentBitPosition[name] = m_componentPools->size();
			m_componentPools->push_back(std::make_unique<SparseSet<T>>());
			//m_componentPools.push_back(std::make_unique<SparseSet<T>>());

			ECS_INFO("Registered component '" << name << "'");
		}

		/*
		*  Attaches a component to an entity
		*
		* - Add<Transform>(player, {x, y, z});
		*/
		template <typename T>
		T& Add(EntityID id, T&& component = {}) 
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			SparseSet<T>& pool = GetComponentPool<T>(true);

			// If component already exists, overwrite
			if (pool.Get(id))
				return *pool.Set(id, std::move(component));

			ComponentMask& mask = GetEntityMask(id);

			// Remove from old group
			RemoveFromGroup(mask, id);

			SetComponentBit<T>(mask, 1);

			// Add ID to new group
			AssignToGroup(mask, id);

			ECS_INFO("Attached '" << typeid(T).name() << "' to " << ENTITY_INFO(id));
			return *pool.Set(id, std::move(component));
		}

		/*
		*  Retrieves the specified component for the given entity
		*
		* - ecs.Get<Transform>(player);
		*/
		template <typename T>
		T& Get(EntityID id) 
		{
			ECS_ASSERT_VALID_ENTITY(id);

			SparseSet<T>& pool = GetComponentPool<T>();
			T* component = pool.Get(id);
			ECS_ASSERT(component,
				ENTITY_INFO(id) << " missing component in '" << typeid(T).name() << "' pool");

			return *component;
		}

		/*
		*  Removes a component from an entity
		*
		* - ecs.Remove<Transform>(player);
		*/
		template <typename T>
		void Remove(EntityID id) 
		{
			ECS_ASSERT_VALID_ENTITY(id);

			SparseSet<T>& pool = GetComponentPool<T>();

			if (!pool.Get(id)) return;

			ComponentMask& mask = GetEntityMask(id);

			// Remove from old group
			RemoveFromGroup(mask, id);

			SetComponentBit<T>(mask, 0);

			// Shift to new group with new mask
			AssignToGroup(mask, id);

			pool.Delete(id);
			ECS_INFO("Removed '" << typeid(T).name() << "' from " << ENTITY_INFO(id));
		}

		template <typename... Ts>
		bool HasAll(EntityID id) 
		{
			// Fold operator, reads as 
			// (HasComponent<Transform> && HasComponent<Physics> && HasComponent<Sprite> && ...)
			return (Has<Ts>(id) && ...);
		}

		template <typename T>
		bool Has(EntityID id) 
		{
			return GetComponentBit<T>(GetEntityMask(id));
		}

		/*
		* Gets all the entity IDs matching the component parameter pack
		*
		* for (EntityID id : ecs.View<Transform, Sprite>()) {
		*   ...
		* }
		*/
		template <typename ...Components>
		std::vector<EntityID> ViewIDs() 
		{
			std::vector<EntityID> result;
			ComponentMask targetMask = GetMask<Components...>();

			for (auto& [mask, ids] : m_Groupings) 
			{
				if ((mask & targetMask) == targetMask)
				{
					result.insert(result.end(), ids.Data().begin(), ids.Data().end());
				}
			}

			return result;
		}

		/*
		*  Returns tuple containing the id of the entity and all
		*  valid components matching the passed parameter pack
		*
		*  for (auto& [id, a, b] : ecs.ViewEach<A, B>()) {
		*    ...
		*  }
		*/
		template <typename ...Components>
		std::vector<std::tuple<EntityID, Components&...>> View() 
		{
			std::vector<std::tuple<EntityID, Components&...>> result;
			ComponentMask targetMask = GetMask<Components...>();

			for (auto& [mask, ids] : m_Groupings) 
			{
				if ((mask & targetMask) == targetMask) 
				{
					for (EntityID id : ids.Data()) 
					{
						result.emplace_back(id, Get<Components>(id)...);
					}
				}
			}

			return result;
		}

		/*
		*  Executes a passed lambda on all the entities that match the
		*  passed parameter pack.
		*
		*  Provided function should follow one of two forms:
		*  [](Component& c1, Component& c2);
		*  OR
		*  [](EntityID id, Component& c1, Component& c2);
		*/
		template <typename ...Components, typename Func>
		void ForEach(Func&& func) 
		{
			std::vector<std::tuple<EntityID, Components&...>> result;
			ComponentMask targetMask = GetMask<Components...>();

			std::vector<SparseSet<EntityID>> groups;

			for (auto& [mask, ids] : m_Groupings) 
			{
				// collect group if mask is a subset of targetMask 
				if ((mask & targetMask) == targetMask) 
				{
					groups.push_back(ids);
				}
			}

			for (auto itr = groups.rbegin(); itr != groups.rend(); itr++) 
			{
				std::vector<EntityID> data = itr->Data();

				for (int i = data.size() - 1; i >= 0; i--) 
				{
					EntityID id = data[i];

					// This branch is for [](EntityID id, Component& c1, Component& c2);
					// constexpr denotes this is evaluated at compile time, which allows
					// the calling of func with different parameters.
					if constexpr (std::is_invocable_v<Func, EntityID, Components&...>) 
					{
						func(id, Get<Components>(id)...);
					}

					// This branch is for [](Component& c1, Component& c2);
					else if constexpr (std::is_invocable_v<Func, Components&...>) 
					{
						func(Get<Components>(id)...);
					}

					else 
					{
						ECS_ASSERT(false, "Bad lambda provided to .ForEach(), parameter pack does not match lambda args");
					}
				}
			}
		}

		size_t GetEntityCount() 
		{
			return m_entityMasks.Size();
		}

		void PrintGroupings() 
		{
			std::stringstream ss;

			for (auto& [mask, sparse] : m_Groupings) 
			{
				// Create string for grouping
				bool findingFirstBit = true;
				std::stringstream maskSS;

				for (int i = MAX_COMPONENTS - 1; i >= 0; i--) 
				{
					if (mask[i] == 0 && findingFirstBit)
						continue;
					findingFirstBit = false;
					maskSS << mask[i];
				}

				ss << "\n" << maskSS.str() << ": ";

				std::stringstream idSS;

				std::string delim = "";

				for (EntityID id : sparse.Data()) 
				{
					idSS << delim << id;
					if (delim.empty()) delim = ", ";
				}

				ss << "[" << idSS.str() << "]";
			}

			ECS_INFO(ss.str());
		}

		void PrintEntityMask(EntityID id) 
		{
			ECS_ASSERT_VALID_ENTITY(id);
			std::stringstream ss;

			ComponentMask& mask = *m_entityMasks.Get(id);

			bool findingFirstBit = true;
			for (int i = MAX_COMPONENTS - 1; i >= 0; i--) 
			{
				if (mask[i] == 0 && findingFirstBit)
				{
					continue;
				}

				findingFirstBit = false;
				ss << mask[i];
			}

			ECS_INFO(ss.str());
		}

		void PrintActive() 
		{
			std::stringstream ss;
			std::string delim = "";

			for (auto& [_, ids] : m_Groupings) 
			{
				for (EntityID id : ids.Data()) 
				{
					ss << delim << id;
					if (delim.empty()) delim = ", ";
				}
			}

			ECS_INFO("\Active entities:\n" << ss.str());
		}

		void PrintEntityComponents(EntityID id) 
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			ComponentMask& mask = GetEntityMask(id);

			std::stringstream ss;
			std::string delim = "";

			for (auto& [name, pos] : m_componentBitPosition)
			{
				if (mask[pos])
				{
					ss << delim << " - " << name;
				}
				if (delim.empty())
				{
					delim = "\n";
				}
			}

			ECS_INFO("\n" << ENTITY_INFO(id) << " components:\n" << ss.str());
		}

	private:
		template <typename T>
		size_t GetComponentBitPosition() 
		{
			TypeName name = typeid(T).name();
			auto it = m_componentBitPosition.find(name);

			if (it == m_componentBitPosition.end())
			{
				return tombstone;
			}
				
			return it->second;
		}

		/*
		* Retrieves reference for the specific component pool given a component name
		*/
		template <typename T>
		SparseSet<T>& GetComponentPool(bool registerIfNotFound = false) 
		{
			size_t bitPos = GetComponentBitPosition<T>();

			if (bitPos == tombstone) 
			{
				if (registerIfNotFound) 
				{
					RegisterComponent<T>();
					bitPos = GetComponentBitPosition<T>();
				}

				ECS_ASSERT(registerIfNotFound,
					"Attempting to operate on unregistered component '" << typeid(T).name() << "'");
			}

			ECS_ASSERT(bitPos < m_componentPools->size() && bitPos >= 0,
				"(Internal): Attempting to index into m_componentPools with out of range bit position");

			// Downcast the generic pointer to the specific sparse set
			ISparseSet* genericPtr = (*m_componentPools)[bitPos].get();
			SparseSet<T>* pool = static_cast<SparseSet<T>*>(genericPtr);

			return *pool;
		}

		template <typename Component>
		void SetComponentBit(ComponentMask& mask, bool val) 
		{
			size_t bitPos = GetComponentBitPosition<Component>();

			ECS_ASSERT(bitPos != tombstone, "Attempting to operate on unregistered component '" << typeid(Component).name() << "'");

			mask[bitPos] = val;
		}

		template <typename Component>
		ComponentMask::reference GetComponentBit(ComponentMask& mask) 
		{
			size_t bitPos = GetComponentBitPosition<Component>();

			ECS_ASSERT(bitPos != tombstone, "Attempting to operate on unregistered component '" << typeid(Component).name() << "'");

			return mask[bitPos];
		}

		ComponentMask& GetEntityMask(EntityID id) 
		{
			ComponentMask* mask = m_entityMasks.Get(id);

			ECS_ASSERT(mask, "Entity " << ENTITY_INFO(id) << " has no component mask");

			return *mask;
		}

		/*
		*  Assembles a generic mask for the given components
		*/
		template <typename... Components>
		ComponentMask GetMask() 
		{
			ComponentMask mask;
			(SetComponentBit<Components>(mask, 1), ...); // fold expression
			return mask;
		}

		// Removes an entity from it's group with its current mask
		void RemoveFromGroup(ComponentMask& mask, EntityID id) 
		{
			if (mask.none()) return;

			SparseSet<EntityID>& group = GetGroupedEntities(mask);
			group.Delete(id);

			// Delete grouping if it's empty
			if (group.IsEmpty())
			{
				m_Groupings.erase(mask);
			}
		}

		void AssignToGroup(ComponentMask& mask, EntityID id) 
		{
			// If mask is empty, no group
			if (mask.none()) return;

			// Create group if it doesn't exist
			m_Groupings.emplace(std::piecewise_construct, std::forward_as_tuple(mask), std::forward_as_tuple()); // Empty sparse set
			m_Groupings[mask].Set(id, id);
		}

		SparseSet<EntityID>& GetGroupedEntities(ComponentMask& mask) 
		{
			auto found = m_Groupings.find(mask);

			ECS_ASSERT(found != m_Groupings.end(), "Cannot find group for entities with mask " << mask);

			return found->second;
		}

	private:
		static constexpr size_t tombstone = std::numeric_limits<size_t>::max();
		
		// List of IDs already created, but no longer in use
		std::vector<EntityID> m_AvailableEntities;
		
		// Holds the component mask for an entity
		SparseSet<ComponentMask> m_entityMasks;
		
		// Groups entities that share a component mask.
		// A, B, C: [1, 2, 3]
		// B: [4]
		std::unordered_map<ComponentMask, SparseSet<EntityID>> m_Groupings;

		// Associates ID with name provided in CreateEntity(), mainly for debugging
		std::unordered_map<EntityID, std::string> m_entityNames;

		// Holds generic pointers to specific component sparse sets.
		// 
		// Index into this array using the corresponding bit position
		// found by using m_componentBitPosition

		// Line 742 causes compiler error c2672 std::construct_at: no matching overloaded function found
		std::vector<std::unique_ptr<ISparseSet>>* m_componentPools = new std::vector<std::unique_ptr<ISparseSet>>;

		// Key is component name, value is the bit position in ComponentMask
		std::unordered_map<TypeName, size_t> m_componentBitPosition;

		// Highest recorded entity ID
		EntityID m_maxEntityID = 0;
	};
}
