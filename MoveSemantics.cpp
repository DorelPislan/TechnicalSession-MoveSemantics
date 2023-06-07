
#include <iostream>
#include <format>

class NonMoveable
{
public:
    //class default c-tor
    NonMoveable()
    {
        mInstanceId = kInstanceId++;

        Trace("Default ctor");
    }

    //class c-tor
    NonMoveable(const char* aRes)
    {
        mInstanceId = kInstanceId++;
        SetResource(aRes);
        Trace("Non-Default ctor");
    }

    //copy c-tor
    NonMoveable(const NonMoveable& aOther)
        : NonMoveable(aOther.mResource)
    {
        mInstanceId = kInstanceId++;
        Trace("Copy ctor");
    }

    //class d-tor
    ~NonMoveable()
    {
        Trace("Destructor");

        delete mResource;
    }

    void SetResource(const char* aNewData)
    {
        size_t newResLength = strlen(aNewData) + 1;
        mResource = new char[newResLength];
        strcpy_s(mResource, newResLength, aNewData);

        Trace("SetResource()");
    }

    NonMoveable& operator=(const NonMoveable & aOther)
    {
        size_t newResLength = strlen(aOther.mResource) + 1;

        char* newData = new char[newResLength];

        strcpy_s(newData, newResLength, aOther.mResource);

        mResource = newData;
        Trace("operator = ");

        return *this;        
    }
private:
    inline static  int kInstanceId = 1;

    char* mResource{};
    int mInstanceId = 0;

    void Trace(const char * aLabel)
    {
        std::cout << std::format("{:25} from instance: {}. Current resource is: \"{}\"\n",
            aLabel, mInstanceId, ( mResource ? mResource : ""));
    }
};


int main()
{
    std::cout << "Hello Move semantics!\n";

    NonMoveable def;
    NonMoveable some("some text");
    NonMoveable copied(some);
}
