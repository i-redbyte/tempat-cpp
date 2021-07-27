#include <cassert>
#include <cstdlib>
#include <iostream>

namespace singleton {

    namespace defaultPolicies {

        template<typename Type>
        class CreateUsingNew {
        public:

            static inline Type *Create() { return new Type; }

            static inline void Destroy(Type *obj) { delete obj; }
        };

        template<typename Type>
        class DefaultLifetime {
        public:

            static inline void OnDeadReference() { throw std::logic_error("Dead reference!"); }

            static inline void ScheduleDestruction(void (*fun)()) { std::atexit(fun); }
        };

        template<typename Type>
        class SingleThreaded {
            struct DummyLock {
            };
        public:
            typedef DummyLock Lock;
            typedef Type VolatileType;
        };
    }

    template<
            typename InstanceT,
            template<typename> class CreationPolicy = defaultPolicies::CreateUsingNew,
            template<typename> class LifetimePolicy = defaultPolicies::DefaultLifetime,
            template<typename> class ThreadingModel = defaultPolicies::SingleThreaded
    >
    class Singleton {
    public:
        static InstanceT &Instance();

    private:
        static void DestroySingleton();

        Singleton() = delete;

        Singleton(const Singleton &) = delete;

        Singleton &operator=(const Singleton &) = delete;

        ~Singleton() = delete;

    private:
        typedef typename ThreadingModel<InstanceT>::VolatileType InstanceType;
        static InstanceType *instance;
        static bool destroyed;
    };

    template<
            typename InstanceT,
            template<typename> class CreationPolicy,
            template<typename> class LifetimePolicy,
            template<typename> class ThreadingModel
    >
    typename Singleton<InstanceT, CreationPolicy, LifetimePolicy, ThreadingModel>::InstanceType *
            Singleton<InstanceT, CreationPolicy, LifetimePolicy, ThreadingModel>::instance = nullptr;

    template<
            typename InstanceT,
            template<typename> class CreationPolicy,
            template<typename> class LifetimePolicy,
            template<typename> class ThreadingModel
    >
    bool Singleton<InstanceT, CreationPolicy, LifetimePolicy, ThreadingModel>::destroyed = false;

    template<
            typename InstanceT,
            template<typename> class CreationPolicy,
            template<typename> class LifetimePolicy,
            template<typename> class ThreadingModel
    >
    InstanceT &Singleton<InstanceT, CreationPolicy, LifetimePolicy, ThreadingModel>::Instance() {
        if (!instance) {
            typename ThreadingModel<InstanceT>::Lock guard;
            (void) guard;
            if (!instance) {
                if (destroyed) {
                    LifetimePolicy<InstanceT>::OnDeadReference();
                    destroyed = false;
                }
                instance = CreationPolicy<InstanceT>::Create();
                LifetimePolicy<InstanceT>::ScheduleDestruction(&DestroySingleton);
            }
        }
        return *instance;
    }

    template<
            typename InstanceT,
            template<typename> class CreationPolicy,
            template<typename> class LifetimePolicy,
            template<typename> class ThreadingModel
    >
    void Singleton<InstanceT, CreationPolicy, LifetimePolicy, ThreadingModel>::DestroySingleton() {
        assert(!destroyed && "Singleton was already destroyed!");
        CreationPolicy<InstanceT>::Destroy(instance);
        instance = nullptr;
        destroyed = true;
    }

}
