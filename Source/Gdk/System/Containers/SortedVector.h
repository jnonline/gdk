/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	// ============================================================================
	// Users should ONLY call AddSorted() to add items to the vector.
	//   the insert() & push_back() methods do not preserve sorting

	template<class T>
	class SortedVector : public std::vector<T>
	{
	public:
		
		void AddSorted(T value)
		{
			this->insert(lower_bound(this->begin(), this->end(), value), value);
		}
	};

} // namespace Gdk
