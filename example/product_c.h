#ifndef _PRODUCT_C_H_
#define _PRODUCT_C_H_

typedef struct rectangle_t
{
   int width;
   int height;
} rectangle_t;

/* 创建一个矩形 */
rectangle_t* rectangle_create(int width, int height);

/* 销毁一个矩形 */
void rectangle_destroy(rectangle_t* rectangle);

/* 求矩形的面积 */
int rectangle_area(rectangle_t* rectangle);

/* 求矩形的对角线长度 */
double rectangle_diag(rectangle_t* rectangle);

const char* rectangle_tostring(rectangle_t* rectangle);
const char* rectangle_tojson(rectangle_t* rectangle);

/* 各个矩形的面积之和 */
int sum_rectangle_area(int count, ...);

#endif
