
#include <format>
#include <iostream>
#include "BlockFormatter.h"

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

  //MoveExperiment(MoveExperiment&& aDisposable) = delete;

  /* * /
  MoveExperiment(MoveExperiment&& aDisposable)
  {
    mInstanceId = kInstanceId++;

    mResource = aDisposable.mResource;

    aDisposable.mResource = nullptr;

    Trace("Move ctor");
  }
  /* */

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

  bool StealFrom(MoveExperiment&& aOther)
  {
    Trace("StealFrom-Before stealing");

    mResource = aOther.mResource;
    aOther.mResource = nullptr;

    Trace("StealFrom-After stealing");

    return true;
  }

private:
  inline static int kInstanceId = 1;

  int mInstanceId = 0;

  char* mResource{};

  void Trace(const char* aLabel)
  {
    std::cout << std::format(
      "{:25} from instance: {:2}. Current resource is: \"{}\"\n",
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
  MoveExperiment some("GetSomethingNonDefault - non-default some");
  some.SetResource("Data set after c-tor some");

  /* */
  int x = 9;
  if (!x)
    return MoveExperiment();
  /*  */

  return some; // Never use std::move to move automatic objects out of
               // functions.
}

void
ReceiveMoveable(MoveExperiment&& aDisposable)
{
  std::cout << "Entering ReceiveMoveable " << std::endl;

  aDisposable.GetResource();

  MoveExperiment local(std::move(aDisposable));
  local.GetResource();
}

int
main()
{
  std::cout << "Hello Move semantics!\n" << std::endl;
  {
    TRACE_BLOCK;
    MoveExperiment def;
    MoveExperiment some("some text");
    MoveExperiment copied(some);
  }
  {
    TRACE_BLOCK;
    MoveExperiment somethingReturned = GetSomething();
  }
  {
    TRACE_BLOCK;
    MoveExperiment somethingConstReturned = GetSomethingConst();
  }
  {
    TRACE_BLOCK;
    MoveExperiment nonDef = GetSomethingNonDefault();
    nonDef.GetResource();
  }
  {
    TRACE_BLOCK;
    ReceiveMoveable(MoveExperiment("inline"));

    MoveExperiment second("second");

    ReceiveMoveable(std::move(second));
  }
  {
    TRACE_BLOCK;
    MoveExperiment m("member function");
    MoveExperiment other("other for member function");

    m.StealFrom(std::move(other));
  }

  {
    /* * /
    struct Foo
    {
      auto func() && {}
    };

    auto a = Foo{};

    a.func(); // Doesn't compile, 'a' is not an rvalue

    std::move(a).func(); // Compiles

    Foo{}.func(); // Compiles
    /* */
  }
}
