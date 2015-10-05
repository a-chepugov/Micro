#ifndef SharedData_h
#define SharedData_h

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))


void abort(int ErrorNum); 

template <typename TTypeName, unsigned char TArrayLength> struct DataArray
{
  TTypeName Array[TArrayLength];

  TTypeName & operator[](char Item)
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

  int Size()
  {
    return arraysize(Array);
  };

  int Length()
  {
    return TArrayLength;
  };
};

typedef unsigned short int Coordinate; // Тип переменной для хранения координаты

enum
{
  Min, Max  // Синонимы положения
};


#endif


