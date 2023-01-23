#include "s21_math.h"

#include <stdio.h>
#include <float.h>

int s21_abs(int x) {
  if (x < 0) {
    x = -x;
  }
  return x;
}

long double s21_ceil(double x) {
  long double integer = (long int)x;
  if (x == -S21_INF) {
    integer = -S21_INF;
  } else if (x == S21_INF) {
    integer = S21_INF;
  } else if (s21_is_nan(x)) {
    integer = S21_NAN;
  } else if (integer != x && x > 0) {
    integer++;
  }
  return integer;
}

long double s21_asin(double x) {
  long double rv = 0;
  int sign = 0;
  if (x < -1 || x > 1) {
    rv = S21_NAN;
  } else {
    if (x < 0) {
      x = -x;
      sign = 1;
    }
    long double denominator = 1;
    long double high = 1;
    long double low = 1;
    long double element = (x * high) / (denominator * low);
    for (int i = 0; i < 500; ++i) {
      rv += element;
      if (low == 1) {
        low = 2;
      }
      if (denominator == 1) {
        denominator = 3;
      }
      element /= low;
      element *= high;
      element /= denominator;
      element *= denominator - 2;
      element *= x * x;
      denominator += 2;
      low += 2;
      high += 2;
    }
  }
  if (x == 1) {
    rv = S21_PI / 2;
  }
  if (sign) {
    rv = -rv;
  }
  return rv;
}

long double s21_acos(double x) {
  long double rv = S21_PI / 2 - s21_asin(x);
  return rv;
}

long double s21_atan(double x) {
  int sign = 0;
  long double rv = 0;
  if (s21_is_nan(x)) {
    rv = S21_NAN;
  } else if (x <= 1 && x >= -1) {
    if (x < 0) {
      sign = 1;
      x = -x;
    }
    long double denominator = 1;
    long double element = x;
    int sign_el = 1;
    for (int i = 0; i < 1000; ++i) {
      rv += sign_el * element;
      sign_el = -sign_el;
      element *= x * x;
      element *= denominator;
      denominator += 2;
      element /= denominator;
    }
    if (sign) {
      rv = -rv;
    }
  } else if (x > 1) {
    rv = S21_PI / 2 - s21_atan(1 / x);
  } else if (x < -1) {
    rv = -S21_PI / 2 - s21_atan(1 / x);
  }
  return rv;
}

long double s21_sin(double x) {
  long double rv = 0;
  int sign = 1;
  if (x == S21_INF) {
    rv = S21_NAN;
  } else if (x == -S21_INF) {
    rv = S21_NAN;
  } else {
    if (x < 0) {
      sign = -1;
      x = -x;
    }
    while (x - (2 * S21_PI) > 0) {
      x -= (2 * S21_PI);
    }
    if (x > S21_PI) {
      x -= S21_PI;
      sign *= -1;
    } else if (x > S21_PI / 2) {
      x = S21_PI - x;
    }
    long double element = x;
    for (int i = 3; i < 1000; i += 2) {
      rv += element;
      element *= x * x;
      element *= -1;
      element /= (i * (i - 1));
    }
  }
  return rv * sign;
}

long double s21_cos(double x) {
  long double rv = 0;
  int sign = 1;
  if (x == S21_INF) {
    rv = S21_NAN;
  } else if (x == -S21_INF) {
    rv = S21_NAN;
  } else {
    if (x < 0) {
      x = -x;
    }
    while (x - (2 * S21_PI) > 0) {
      x -= (2 * S21_PI);
    }
    if (x > S21_PI) {
      x -= S21_PI;
      sign *= -1;
    }
    long double element = 1;
    for (int i = 2; i < 1000; i += 2) {
      rv += element;
      element *= x * x;
      element *= -1;
      element /= (i * (i - 1));
    }
    if (x == S21_PI / 2) {
      rv = 0;
    }
  }
  return rv * sign;
}

long double s21_tan(double x) {
  long double rv = 0;
  if (x == S21_INF) {
    rv = S21_NAN;
  } else if (x == -S21_INF) {
    rv = S21_NAN;
  } else {
    while (x - S21_PI > 0) {
      x -= S21_PI;
    }
    if (x == S21_PI / 2 || x == -S21_PI / 2) {
      rv = TAN_PIBY2;
    } else {
      rv = s21_sin(x) / s21_cos(x);
    }
  }
  return rv;
}

long double s21_log(double x) {
  long double rv = 0;
  if (x == S21_INF) {
    rv = S21_INF;
  } else if (x < 0) {
    rv = S21_NAN;
  } else if (x <= 1) {
    x -= 1;
    long double element = x;
    long double denominator = 1;
    for (int i = 0; i < 500; ++i) {
      rv += element;
      element *= -1;
      element *= x;
      denominator += 1;
      element /= denominator;
      element *= denominator - 1;
    }
  } else {
    long double y = (1 - x) / (1 + x);
    long double element = -2 * y;
    long double denominator = 1;
    for (int i = 0; i < 10000; ++i) {
      rv += element;
      element *= y * y;
      denominator += 2;
      element /= denominator;
      element *= denominator - 2;
    }
  }
  return rv;
}

long double s21_pow(double base, double exp) {
  long double rv = 0;
  long double log_base = s21_log(base);
  long double arg = log_base * exp;
  rv = s21_exp(arg);
  if (base == 0) {
    rv = 0;
  }
  return rv;
}

long double s21_exp(double x) {
  long double rv = 0;
  if (x == S21_INF) {
    rv = S21_INF;
  } else if (x == -S21_INF) {
    rv = 0;
  } else if (x > LDBL_MAX_10_EXP * 2.3) {
    rv = S21_INF;
  } else if (x < LDBL_MIN_10_EXP * 2.3) {
    rv = 0;
  } else {
    long double element = 1;
    for (int i = 1; i < 100000; ++i) {
      rv += element;
      element *= x;
      element /= i;
    }
  }
  return rv;
}

long double s21_sqrt(double x) {
  long double rv = 0;
  rv = s21_pow(x, 0.5);
  return rv;
}

long double s21_fabs(double x) { return (x >= 0 ? x : -x); }

long double s21_floor(double x) {
  long double int_x = (long long int)x;
  if (x == S21_INF) {
    int_x = S21_INF;
  } else if (x == -S21_INF) {
    int_x = -S21_INF;
  } else if (s21_is_nan(x)) {
    int_x = S21_NAN;
  } else if ((x != int_x) && x < 0) {
    if (x > -1) {
      int_x = -1;
    } else {
      int_x = int_x - 1;
    }
  }
  return int_x;
}

long double s21_fmod(double x, double y) {
  long double rv = 0;
  long double copy_x = x;
  long double copy_y = y;
  long double count = 0;
  if (y == 0 || s21_is_nan(x) || s21_is_nan(y)) {
    rv = S21_NAN;
  } else if (x == y) {
    copy_x = 0;
  } else if (x == 0) {
    rv = 0;
  } else if (x > 0 && y > 0) {
    while (x - y > 0) {
      x -= y;
      count++;
    }
  } else if (x < 0 && y > 0) {
    x = -x;
    while (x - y > 0) {
      x -= y;
      count++;
    }
    count *= -1;
  } else if (y < 0 && x > 0) {
    y = -y;
    while (x - y > 0) {
      x -= y;
      count++;
    }
    count *= -1;
  }
  if (!s21_is_nan(rv)) {
    rv = copy_x - count * copy_y;
  }
  return rv;
}

int s21_is_nan(double x) { return x != x; }
