#ifndef ROCKETMOD_QMM_VHOOK_HPP
#define ROCKETMOD_QMM_VHOOK_HPP

#include <cstring>
#include <vector>

class VHook {
    private:
    public:
        void* p;
        void* vtable_orig;
        std::vector<void*> vtable_copy;

        int get_vtable_size() {
            void** vtable = (void**)vtable_orig;

            int retval = 0;
            while (vtable[retval] != 0)
                retval++;

            return retval;
        }

    public:
        VHook() : p(nullptr), vtable_orig(nullptr) {
        }

        ~VHook() {
            vtable_copy.clear();
            if (vtable_orig)
                *(void**)p = vtable_orig;
        }

        bool copy_vtable(void* obj, size_t sz = 0) {
            if (obj == nullptr)
                return false;

            p = obj;
            vtable_orig = *(void**)p;

            if (!sz)
                sz = get_vtable_size();

            vtable_copy.resize(sz);
            memcpy(vtable_copy.data(), vtable_orig, sz * sizeof(void*));

            *(void**)p = vtable_copy.data();

            return true;
        }

        void* set_vtable_index(int index, void* hook_func) {
            if (!p || !vtable_orig || vtable_copy.empty() || !hook_func || index >= vtable_copy.size())
                return nullptr;

            void* old_func = vtable_copy[index];

            vtable_copy[index] = hook_func;

            return old_func;
        }
};

#endif // ROCKETMOD_QMM_VHOOK_HPP
