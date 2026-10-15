#ifndef ROCKETMOD_QMM_VHOOK_HPP
#define ROCKETMOD_QMM_VHOOK_HPP

#include <cstring>
#include <vector>

/**
* @brief Modification for a single object's vtable.
* 
* Create a VHook object. Then call vhook.copy_vtable(&obj) to make a copy of obj's vtable ready for hooking.
* 
* To inject a specific hook into obj's new vtable, call old_func = vhook.set_vtable_index(index, hook_func).
* 
* You can undo a single hook by calling the set_vtable_index function again with the old returned function pointer.
*
* Upon VHook destruction, obj's vtable will be replaced with the original.
*/
class VHook {
    private:
    public:
        void* self;
        void* vtable_orig;
        std::vector<void*> vtable_copy;

        /**
        * @brief Get number of function pointers in vtable
        *
        * @return Number of function pointers in vtable
        */
        int get_vtable_size() {
            // loop through vtable for a null pointer to get max size
            void** vtable = (void**)vtable_orig;

            int retval = 0;
            while (vtable[retval] != 0)
                retval++;

            return retval;
        }

    public:
        VHook() : self(nullptr), vtable_orig(nullptr) {
        }

        ~VHook() {
            vtable_copy.clear();
            // restore original vtable pointer to object
            if (vtable_orig)
                *(void**)self = vtable_orig;
        }

        /**
        * @brief Initialize a new vtable copy for a given object.
        *
        * @param obj Obj to copy the vtable of.
        * @param sz Size of vtable. If left 0, this will be auto-detected.
        * @return true If vtable copy was successful.
        */
        bool copy_vtable(void* obj, size_t sz = 0) {
            if (obj == nullptr)
                return false;

            self = obj;
            vtable_orig = *(void**)obj;

            if (!sz)
                sz = get_vtable_size();

            // make copy of vtable
            vtable_copy.resize(sz);
            memcpy(vtable_copy.data(), vtable_orig, sz * sizeof(void*));

            // have object use vtable copy
            *(void**)obj = vtable_copy.data();

            return !vtable_copy.empty();
        }

        /**
        * @brief Install a new function pointer into the copied vtable.
        *
        * @param index Index into vtable to assign new function pointer.
        * @param hook_func New function pointer.
        * @return The original function pointer. nullptr if failure.
        */
        void* set_vtable_index(size_t index, void* hook_func) {
            if (!self || !vtable_orig || vtable_copy.empty() || !hook_func || index >= vtable_copy.size())
                return nullptr;

            void* old_func = vtable_copy[index];

            vtable_copy[index] = hook_func;

            return old_func;
        }
};

#endif // ROCKETMOD_QMM_VHOOK_HPP
