
#include <format>
#include <iostream>

class MoveExperiment
{
public:
  // class default c-tor
  MoveExperiment()
  {
    mInstanceId = kInstanceId++;

    Trace("Default ctor");
  }

  // class c-tor
  MoveExperiment(const char* aRes)
  {
    mInstanceId = kInstanceId++;
    SetResource(aRes, false);
    Trace("Non-Default ctor");
  }

  // copy c-tor
  /* */
  MoveExperiment(const MoveExperiment& aOther)
    : MoveExperiment(aOther.mResource)
  {
    mInstanceId = kInstanceId++;
    Trace("Copy ctor");
  }
  /* */

  // class d-tor
  ~MoveExperiment()
  {
    Trace("Destructor");

    delete mResource;
  }

  MoveExperiment(MoveExperiment&& aDisposable) = delete;
  /*
  {
    mInstanceId = kInstanceId++;

    mResource = aDisposable.mResource;

    aDisposable.mResource = nullptr;

    Trace("Move ctor");
  }
  */

  MoveExperiment& operator=(MoveExperiment&& aDisposable) = delete;

  void SetResource(const char* aNewData, bool aTrace = true)
  {
    size_t newResLength = strlen(aNewData) + 1;
    char* newRes = new char[newResLength];
    strcpy_s(newRes, newResLength, aNewData);

    delete mResource;

    mResource = newRes;

    if (aTrace)
      Trace("SetResource()");
  }

  const char* GetResource()
  {
    Trace("GetResource() ");
    return mResource;
  }

  MoveExperiment& operator=(const MoveExperiment& aOther)
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

MoveExperiment
GetSomething()
{
  return MoveExperiment("GetSomething-default");
}

const MoveExperiment
GetSomethingConst()
{
  return MoveExperiment("GetSomethingConst - returned something const");
}

MoveExperiment
GetSomethingNonDefault()
{
  MoveExperiment some("GetSomethingNonDefault - non-default");
  some.SetResource("Data set after c-tor");

  int x = 9;
  if (!x)
    return MoveExperiment();

  return some; // Never use std::move to move automatic objects out of
               // functions.
}

int
main()
{
  std::cout << "Hello Move semantics!\n";
  {
    MoveExperiment def;
    MoveExperiment some("some text");
    MoveExperiment copied(some);
  }
  std::cout << std::endl << std::endl;

  {
    MoveExperiment somethingReturned = GetSomething();
  }
  std::cout << std::endl << std::endl;

  {
    MoveExperiment somethingConstReturned = GetSomethingConst();
  }
  std::cout << std::endl << std::endl;
  {
    MoveExperiment nonDef = GetSomethingNonDefault();
    nonDef.GetResource();
  }
  std::cout << std::endl << std::endl;
}
