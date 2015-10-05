int CharToInt(char *Array) { // Функция преобразования массива char в int (Старший и младший байты поменяны местами!!!)
  int *result;
  result = (int *) Array;
  return *result;
};

/*
int CharToInt(char *Array) { // Функция преобразования массива char в int
  int result = 0;
  for (char i = 0; i < sizeof(Array); i++)
  {
    result = result * 256 + Array[i];
  }
  return result;
};
*/
