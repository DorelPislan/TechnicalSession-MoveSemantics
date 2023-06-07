
#include <format>
#include <iostream>

class NonMoveable
{
public:
  // class default c-tor
  NonMoveable()
  {
    mInstanceId = kInstanceId++;

    Trace("Default ctor");
  }

  // class c-tor
  NonMoveable(const char* aRes)
  {
    mInstanceId = kInstanceId++;
    SetResource(aRes);
    Trace("Non-Default ctor");
  }

  // copy c-tor
  NonMoveable(const NonMoveable& aOther)
    : NonMoveable(aOther.mResource)
  {
    mInstanceId = kInstanceId++;
    Trace("Copy ctor");
  }

  // class d-tor
  ~NonMoveable()
  {
    Trace("Destructor");

    delete mResource;
  }

  NonMoveable(NonMoveable&& aDisposable) = delete;

  void SetResource(const char* aNewData)
  {
    size_t newResLength = strlen(aNewData) + 1;
    char* newRes = new char[newResLength];
    strcpy_s(newRes, newResLength, aNewData);

    delete mResource;

    mResource = newRes;

    // Trace("SetResource()");
  }

  const char* GetResource() { return mResource; }

  NonMoveable& operator=(const NonMoveable& aOther)
  {
    if (this != &aOther) {
      size_t newResLength = strlen(aOther.mResource) + 1;

      char* newData = new char[newResLength];

      strcpy_s(newData, newResLength, aOther.mResource);

      delete mResource;
      mResource = newData;

      Trace("operator = ");
    }
    return *this;
  }
  /*
  NonMoveable& operator=(NonMoveable aOther) // unified assignment operator
  {
    swap(aOther);
    return *this;
  }
  */

private:
  inline static int kInstanceId = 1;

  char* mResource{};
  int mInstanceId = 0;

  void Trace(const char* aLabel)
  {
    std::cout << std::format(
      "{:25} from instance: {}. Current resource is: \"{}\"\n",
      aLabel,
      mInstanceId,
      (mResource ? mResource : ""));
  }
};

NonMoveable
GetSomething()
{
  return NonMoveable("GetSomething-default");
}

const NonMoveable
GetSomethingConst()
{
  return NonMoveable("GetSomethingConst - returned something const");
}

NonMoveable
GetSomethingNonDefault()
{
  NonMoveable some("GetSomethingNonDefault - non-default");
  some.SetResource("Data set after c-tor");

  return some; // Never use std::move to move automatic objects out of
               // functions.
}

int
main()
{
  std::cout << "Hello Move semantics!\n";
  {
    NonMoveable def;
    NonMoveable some("some text");
    NonMoveable copied(some);
  }
  std::cout << std::endl << std::endl;

  {
    NonMoveable somethingReturned = GetSomething();
  }
  std::cout << std::endl << std::endl;

  {
    NonMoveable somethingConstReturned = GetSomethingConst();
  }
  std::cout << std::endl << std::endl;
  {
    NonMoveable nonDef = GetSomethingNonDefault();
    nonDef.GetResource();
  }
  std::cout << std::endl << std::endl;
}
