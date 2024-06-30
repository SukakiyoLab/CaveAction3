#pragma once

namespace pointer_wrapper {
	template<typename T>
	void Delete(T* data_ptr) {
		delete data_ptr;
		data_ptr = nullptr;
	}
}
