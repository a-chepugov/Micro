#ifndef SharedData_h
#define SharedData_h
//==============================================================================

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

template <class TTypeName, unsigned short int TArrayLength> class DataArray
{
private:
  TTypeName Array[TArrayLength];
public:
  TTypeName & operator[](unsigned short int Item)
  {
    if (Item >= 0 && Item < TArrayLength )
    {
      return Array[Item];
    }
    else
    {
      ::abort();
    };
  };

  unsigned int Size()
  {
    return arraysize(Array);
  };

  unsigned int Length()
  {
    return TArrayLength;
  };

  TTypeName * Pointer()
  {
    return Array;
  };
};

typedef short int Coordinate; // Тип переменной для хранения координаты

enum
{
  Min, Max  // Синонимы положения
};

//==============================================================================
#endif





