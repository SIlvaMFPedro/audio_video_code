#include <opencv2/opencv.hpp>

using namespace cv;

/**
 * @brief Get the current sample from the frame {@code matrix} in the Y plane, considering {@code matrix[row, column]},
 * where the current sample is located in {@code matrix[row, column]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of the current sample.
 * @param column the column of our point, reference of the current sample.
 * @return uchar the sample in the position of current sample.
 */
uchar getYSample(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample A from the frame {@code matrix} in the Y plane, considering {@code matrix[row, column]},
 * where A is located in {@code matrix[row, column - 1]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of A.
 * @param column the column of our point, reference of A.
 * @return uchar the sample in position A.
 */
uchar getYSampleA(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample B from the frame {@code matrix} in the Y plane, considering {@code matrix[row, column]},
 * where B is located in {@code matrix[row - 1, column]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of B.
 * @param column the column of our point, reference of B.
 * @return uchar the sample in position B.
 */
uchar getYSampleB(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample C from the frame {@code matrix} in the Y plane, considering {@code matrix[row, column]},
 * where C is located in {@code matrix[row - 1, column - 1]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of C.
 * @param column the column of our point, reference of C.
 * @return uchar the sample in position C.
 */
uchar getYSampleC(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the current sample from the frame {@code matrix} in the U plane, considering {@code matrix[row, column]},
 * where the current sample is located in {@code matrix[row, column]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of the current sample.
 * @param column the column of our point, reference of the current sample.
 * @return uchar the sample in the position of current sample.
 */
uchar getUSample(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample A from the frame {@code matrix} in the U plane, considering {@code matrix[row, column]},
 * where A is located in {@code matrix[row, column - 1]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of A.
 * @param column the column of our point, reference of A.
 * @return uchar the sample in position A.
 */
uchar getUSampleA(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample B from the frame {@code matrix} in the U plane, considering {@code matrix[row, column]},
 * where B is located in {@code matrix[row - 1, column]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of B.
 * @param column the column of our point, reference of B.
 * @return uchar the sample in position B.
 */
uchar getUSampleB(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample C from the frame {@code matrix} in the U plane, considering {@code matrix[row, column]},
 * where C is located in {@code matrix[row - 1, column - 1]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of C.
 * @param column the column of our point, reference of C.
 * @return uchar the sample in position C.
 */
uchar getUSampleC(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the current sample from the frame {@code matrix} in the V plane, considering {@code matrix[row, column]},
 * where the current sample is located in {@code matrix[row, column]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of the current sample.
 * @param column the column of our point, reference of the current sample.
 * @return uchar the sample in the position of current sample.
 */
uchar getVSample(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample A from the frame {@code matrix} in the V plane, considering {@code matrix[row, column]},
 * where A is located in {@code matrix[row, column - 1]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of A.
 * @param column the column of our point, reference of A.
 * @return uchar the sample in position A.
 */
uchar getVSampleA(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample B from the frame {@code matrix} in the V plane, considering {@code matrix[row, column]},
 * where B is located in {@code matrix[row - 1, column]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of B.
 * @param column the column of our point, reference of B.
 * @return uchar the sample in position B.
 */
uchar getVSampleB(uchar matrix[], Size size, int row, int column, int type);

/**
 * @brief Get the sample C from the frame {@code matrix} in the V plane, considering {@code matrix[row, column]},
 * where C is located in {@code matrix[row - 1, column - 1]}.
 *
 * @param matrix the frame where the samples are located.
 * @param size the size of the frame, with type {@code cv::Size}.
 * @param row the row of our point, reference of C.
 * @param column the column of our point, reference of C.
 * @return uchar the sample in position C.
 */
uchar getVSampleC(uchar matrix[], Size size, int row, int column, int type);

void putYSample(uchar matrix[], Size size, int row, int column, int type, uchar sample);
void putUSample(uchar matrix[], Size size, int row, int column, int type, uchar sample);
void putVSample(uchar matrix[], Size size, int row, int column, int type, uchar sample);

uchar getDecodedYSampleA(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedYSampleB(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedYSampleC(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedUSampleA(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedUSampleB(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedUSampleC(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedVSampleA(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedVSampleB(std::vector<uchar>* matrix, Size size, int row, int column, int type);

/**
 *
 * @param matrix
 * @param size
 * @param row
 * @param column
 * @param type
 * @return
 */
uchar getDecodedVSampleC(std::vector<uchar>* matrix, Size size, int row, int column, int type);
