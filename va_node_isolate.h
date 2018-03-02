#ifndef VA_Node_Isolate_Interface
#define VA_Node_Isolate_Interface

/************************
 *****  Components  *****
 ************************/

#include "va_node_entity.h"

/**************************
 *****  Declarations  *****
 **************************/

#include "va_node_process.h"

/*************************
 *****  Definitions  *****
 *************************/

namespace VA {
    namespace Node {
        class Export;
        class Isolated;

        class Isolate : public Entity {
            DECLARE_CONCRETE_RTTLITE (Isolate, Entity);

            friend class Process::Primary;

            friend class Isolated;
            friend class Export;

        //  Aliases
        public:
            typedef v8::Isolate object_t;
            typedef object_t*   handle_t;
            typedef object_t*   global_t;

            typedef v8::NativeWeakMap              object_cache_t;
            typedef V8<object_cache_t>::local      object_cache_handle_t;
            typedef V8<object_cache_t>::persistent object_cache_global_t;

        //  Construction
        private:
            Isolate (v8::Isolate *pIsolate);

        //  Destruction
        private:
            ~Isolate ();

        //  Instantiation
        public:
            static bool GetInstance (Reference &rpInstance, v8::Isolate *pIsolate);

        //  Decommisioning
        protected:
            bool onDeleteThis ();                             // ... myself
            bool okToDecommision (Isolated *pIsolated) const; // ... others

        //  Access
        public:
            handle_t isolate () const {
                return m_hIsolate;
            }
            handle_t handle () const {
                return m_hIsolate;
            }
            operator handle_t () const {
                return m_hIsolate;
            }
            local_context_t currentContext () const {
                return m_hIsolate->GetCurrentContext ();
            }

        //  Local Access
        public:
        //  ... handle -> local handle
            template <typename source_t> typename V8<source_t>::local GetLocalFor (
                source_t const &rhSouce
            ) const {
                return V8<source_t>::local::New (m_hIsolate, rhSouce);
            }

        //  ... handle -> local handle (maybe)
            template <typename local_t> bool GetLocalFrom (
                local_t &rhLocal, persistent_value_t const &hValue
            ) const {
                return GetLocalFrom (rhLocal, GetLocalFor (hValue));
            }
            template <typename local_t> bool GetLocalFrom (local_t &rhLocal, local_value_t const &hValue) const {
                typename V8<local_t>::maybe hMaybe;
                return GetMaybeFrom (hMaybe, hValue) && ToLocalFrom (rhLocal, hMaybe);
            }

        //  ... handle -> local handle (maybe) fixed points
            bool GetLocalFrom (local_value_t &rhLocal, local_value_t const &hValue) const {
                rhLocal = hValue;
                return true;
            }

        //  ... handle -> maybe local handle fixed points
            bool GetMaybeFrom (V8<v8::Value>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue;
                return !rhMaybe.IsEmpty ();
            }

        //  ... handle -> maybe local handle
            bool GetMaybeFrom (V8<v8::Boolean>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue->ToBoolean (currentContext ());
                return !rhMaybe.IsEmpty ();
            }
            bool GetMaybeFrom (V8<v8::Number>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue->ToNumber (currentContext ());
                return !rhMaybe.IsEmpty ();
            }
            bool GetMaybeFrom (V8<v8::String>::maybe &rhMaybe, local_value_t hValue, bool bToDetailString) const {
                rhMaybe = bToDetailString
                    ? hValue->ToDetailString (currentContext ())
                    : hValue->ToString (currentContext ());
                return !rhMaybe.IsEmpty ();
            }
            bool GetMaybeFrom (V8<v8::Object>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue->ToObject (currentContext ());
                return !rhMaybe.IsEmpty ();
            }
            bool GetMaybeFrom (V8<v8::Integer>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue->ToInteger (currentContext ());
                return !rhMaybe.IsEmpty ();
            }
            bool GetMaybeFrom (V8<v8::Uint32>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue->ToUint32 (currentContext ());
                return !rhMaybe.IsEmpty ();
            }
            bool GetMaybeFrom (V8<v8::Int32>::maybe &rhMaybe, local_value_t hValue) const {
                rhMaybe = hValue->ToInt32 (currentContext ());
                return !rhMaybe.IsEmpty ();
            }

        //  Access Helpers
        public:
            bool UnwrapString (
                VString &rString, maybe_value_t hValue, bool bDetail = false
            ) const;
            bool UnwrapString (
                VString &rString, local_value_t hValue, bool bDetail = false
            ) const;
            bool UnwrapString (VString &rString, maybe_string_t hString) const;
            bool UnwrapString (VString &rString, local_string_t hString) const;

        //  Creation Helpers
        public:
            local_resolver_t NewResolver () const;

            local_string_t NewString (char const *pString) const;

        //  Exception Helpers
        public:
            void ThrowTypeError (char const *pMessage) const;

        //  Export Access
            bool GetExport (Vxa::export_return_t &rExport, local_value_t hValue);

        //  Model Management
        public:
            bool Attach (
                ClassTraits<Export>::retaining_ptr_t &rpModelObject, maybe_value_t hValue
            );
            bool Attach (
                ClassTraits<Export>::retaining_ptr_t &rpModelObject, local_value_t hValue
            );
        private:
            bool Detach (Export *pModelObject);

        //  State
        private:
            global_t        const m_hIsolate;
            object_cache_global_t m_hValueCache;
        };

    } // namespace VA::Node

} // namespace VA


#endif
