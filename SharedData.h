#ifndef SharedData_h
#define SharedData_h

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraylength(array) (sizeof(ArraySizeHelper(array)))

void abort(int ErrorNum); 

template <typename TTypeName, unsigned char TArrayLength> struct DataArray
{
  TTypeName Array[TArrayLength];

  inline TTypeName & operator[](char Item)
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

  inline int Length()
  {
    return arraylength(Array);
  };
};


typedef short int Coordinate; // Тип переменной для хранения координаты


enum
{
  ScanStart, ScanEnd  // Синонимы положения
};

#endif
