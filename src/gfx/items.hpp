#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define what represents an Item, this can be used throughout the code, and is considered the main definition of what an item is.
* Whether it is 3D or 2D.
* ==============================================-
*/

#include <string>
#include <vector>
#include <utility>

#include "sys/types.hpp"


namespace gfx
{

	struct ItemType
	{
		std::string name;
	};

	class ItemTypeManager
	{
	public:

		static const ItemTypeManager& get() noexcept
		{
			static ItemTypeManager instance{};

			return instance;
		}

		const ItemType& get_type(types::type_id id) const noexcept
		{
			return m_item_types[static_cast<size_t>(id)];
		}


	private:

		explicit ItemTypeManager() noexcept
		{
			m_item_types.push_back({ .name{"void"} });
		}


		std::vector<ItemType> m_item_types{};


	};



	class ItemStack
	{
	public:

		enum Source : u8
		{
			Undefined,
			Voxel,
			Item,
		};

		using Type = std::pair<types::type_id, Source>;


		ItemStack(Type type = {}, u16 max_count = {}, u16 count = {}) noexcept
		{
			set(type, max_count, count);
		}

		DEFAULT_COPY_INIT(ItemStack);
		DEFAULT_MOVE_INIT(ItemStack);


		/// <summary>
		/// Add the specified count, if types match
		/// <para> 0 is returned if the types do not match </para> 
		/// </summary>
		/// <param name="tid"></param>
		/// <param name="count"></param>
		/// <returns>The remainder, 0 otherwise</returns>
		u16 add(Type type, u16 count) noexcept;

		/// <summary>
		/// Remove the specified count, if types match
		/// <para> 0 is returned if the types do not match </para> 
		/// </summary>
		/// <param name="tid"></param>
		/// <param name="count"></param>
		/// <returns>How much could be taken</returns>
		u16 take(Type type, u16 count) noexcept;

		/// <summary> 
		/// Clear the item stack as well
		/// <para> 0 is returned if the tyoes do not match </para>
		/// </summary>
		/// <returns>The count of *this</returns>
		u16 take_all() noexcept
		{
			const auto ret = m_count;
			clear();
			return ret;
		}

		/// <summary>
		/// Clear the tid and the count of *this
		/// </summary>
		void clear() noexcept
		{
			m_count = 0;
			m_type.first = {};
			m_type.second = Undefined;
		}


		Type get_type() const noexcept { return m_type; }


		void set_maxCount(u16 max_count) noexcept
		{
			m_max_count = max_count;
		}

		/// <summary>
		/// Reset the count when the tid changes
		/// </summary>
		/// <param name="tid"></param>
		void set_typeId(types::type_id tid) noexcept
		{
			m_count = 0;
			m_type.first = tid;
		}

		void set(Type type, u16 max_count, u16 count) noexcept
		{
			m_type = type;
			m_max_count = max_count;
			m_count = count;
		}


	private:

		Type m_type{};

		u16 m_max_count{};
		u16 m_count{};


	};


}