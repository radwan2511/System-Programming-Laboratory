int digit_counter(char* str) {
  int count = 0;
  while(*str) {
    if (*str <= 57 && *str >= 48) /* 48 = '0', 57 = '9' */
      count++;
    str++; 
  }
  return count;
}
int main() {
  return 0;
}