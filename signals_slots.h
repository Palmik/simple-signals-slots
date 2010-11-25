#include <stdexcept>
#include <functional>
#include <algorithm>
#include <vector>

template<class> class Slot;

template<typename R, typename... ArgTypes>
class Slot<R(ArgTypes...)>
{
    public:
        typedef std::function<R(ArgTypes...)> FunctorType;
        
    public:
        Slot(const FunctorType& callable) : callable_m(callable) {}
        
        void invoke(ArgTypes... args) const
        {
            callable_m(args...);
        }
        
    private:
        FunctorType callable_m;
};

template<class> class Signal;

template<typename R, typename... ArgTypes>
class Signal<R(ArgTypes...)>
{
    typedef Slot<R(ArgTypes...)> SlotType;
    
    public:
        void emit(ArgTypes... a) const
        {
            std::for_each(connectedSlots_m.begin(), connectedSlots_m.end(), std::bind(emitSlot, std::placeholders::_1, a...));
        }
        
        void connect(const typename SlotType::FunctorType& newSlot)
        {
            connectedSlots_m.push_back(SlotType(newSlot));
        }
        
        void disconnect()
        {
            connectedSlots_m.clear();
        }
        
    private:
        static void emitSlot(const SlotType& slot, ArgTypes... a)
        {
            slot.invoke(a...);
        }
        
        std::vector<SlotType> connectedSlots_m;
}; 
