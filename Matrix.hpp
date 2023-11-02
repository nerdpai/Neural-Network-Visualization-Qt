#ifndef MATRIX_2D_HPP
#define MATRIX_2D_HPP


#include<vector>
#include<algorithm>
#include<ostream>
#include<iterator>




#ifndef VECTOR_OUTPUT
#define VECTOR_OUTPUT
template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector)
{
	out << "< ";
	std::copy(vector.begin(), vector.end(), std::ostream_iterator<T>(out, " "));
	out << ">";
	return out;
}
#endif





template<class Type>
class Matrix_2d
{
	class MatrixException : public std::exception { public: MatrixException(char const* const _Message) : std::exception(_Message) {} };
	const int m_colCount;
	const int m_rowCount;
	std::vector<std::vector<Type>> m_array;

public:
	Matrix_2d(const int row_count, const int col_count) : m_rowCount(row_count), m_colCount(col_count), m_array(row_count, std::vector<Type>(col_count, Type{})) {
		if (m_rowCount <= 0) throw MatrixException("ivalid numbers of row");
		else if (m_colCount <= 0) throw MatrixException("ivalid numbers of col");
	}
	Matrix_2d(const int row_count, const int col_count, const Type& data) : m_rowCount(row_count), m_colCount(col_count), m_array(row_count, std::vector<Type>(col_count, data)) {
		if (m_rowCount <= 0) throw MatrixException("ivalid numbers of row");
		else if (m_colCount <= 0) throw MatrixException("ivalid numbers of col");
	}
	Matrix_2d(const Matrix_2d& matrix) : m_rowCount(matrix.m_rowCount), m_colCount(matrix.m_colCount)
	{ *this = matrix; }
	Matrix_2d(Matrix_2d&& matrix) noexcept : m_rowCount(matrix.m_rowCount), m_colCount(matrix.m_colCount)
	{ *this = std::move(matrix); }

	Matrix_2d& operator=(const Matrix_2d& matrix) {
		if (matrix.m_rowCount != m_rowCount || matrix.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		m_array = matrix.m_array;
		return *this;
	}
    Matrix_2d& operator=(Matrix_2d&& matrix) {
		if (matrix.m_rowCount != m_rowCount || matrix.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		m_array = std::move(matrix.m_array);
		return *this;
	}

	//--------------------------------------------------------------------------------------------- scalar

	Matrix_2d operator+(const Type& data) const
	{
		auto temp = *this;
		temp += data;
		return temp;
	}
	Matrix_2d& operator+=(const Type& data)
	{
		std::for_each(m_array.begin(), m_array.end(), [data](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [data](auto& element) {
				element += data;
				});
			});
		return *this;
	}

	Matrix_2d operator*(const Type& data) const
	{
		auto temp = *this;
		temp *= data;
		return temp;
	}
	Matrix_2d& operator*=(const Type& data)
	{
		std::for_each(m_array.begin(), m_array.end(), [data](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [data](auto& element) {
				element *= data;
				});
			});
		return *this;
	}

	Matrix_2d operator/(const Type& data) const
	{
		auto temp = *this;
		temp /= data;
		return temp;
	}
	Matrix_2d& operator/=(const Type& data)
	{
		std::for_each(m_array.begin(), m_array.end(), [data](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [data](auto& element) {
				element /= data;
				});
			});
		return *this;
	}

	Matrix_2d operator-(const Type& data) const
	{
		auto temp = *this;
		temp -= data;
		return temp;
	}
	Matrix_2d& operator-=(const Type& data)
	{
		std::for_each(m_array.begin(), m_array.end(), [data](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [data](auto& element) {
				element -= data;
				});
			});
		return *this;
	}

	Matrix_2d operator%(const Type& data) const
	{
		auto temp = *this;
		temp %= data;
		return temp;
	}
	Matrix_2d& operator%=(const Type& data)
	{
		std::for_each(m_array.begin(), m_array.end(), [data](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [data](auto& element) {
				element %= data;
				});
			});
		return *this;
	}

	Matrix_2d operator-() const
	{
		auto temp = *this;
		std::for_each(temp.m_array.begin(), temp.m_array.end(), [](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [](auto& element) {
				element *= -1;
				});
			});
		return temp;
	}

	//--------------------------------------------------------------------------------------------- another matrix


	Matrix_2d operator+(const Matrix_2d& right)
	{
		if (right.m_rowCount != m_rowCount || right.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		auto temp = *this;
		temp += right;
		return temp;
	}
	Matrix_2d& operator+=(const Matrix_2d& right)
	{
		if (right.m_rowCount != m_rowCount || right.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		for (int i = 0; i < m_rowCount; i++)
		{
			for (int j = 0; j < m_colCount; j++)
				m_array[i][j] += right.m_array[i][j];
		}
		return *this;
	}

	Matrix_2d operator-(const Matrix_2d& right)
	{
		if (right.m_rowCount != m_rowCount || right.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		auto temp = *this;
		temp -= right;
		return temp;
	}
	Matrix_2d& operator-=(const Matrix_2d& right)
	{
		if (right.m_rowCount != m_rowCount || right.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		*this += -right;
		return *this;
	}

	Matrix_2d operator*(const Matrix_2d& right) const
	{
		if (right.m_rowCount != m_colCount)
			throw MatrixException("ivalid matrix");

		Matrix_2d temp(m_rowCount, right.m_colCount);
		for (int i = 0; i < m_rowCount; i++)
		{
			for (int j = 0; j < right.m_colCount; j++)
			{
				for (int k = 0; k < m_colCount; k++)
					temp.m_array[i][j] += m_array[i][k] * right.m_array[k][j];
			}
		}
		return temp;
	}

	//--------------------------------------------------------------------------------------------- special functionality

	Matrix_2d T() const // Transposed matrix
	{
		Matrix_2d temp(m_colCount,m_rowCount);
		for (int i = 0; i < m_colCount; i++)
		{
			for (int j = 0; j < m_rowCount; j++)
				temp.m_array[i][j] = m_array[j][i];
		}
		return temp;
	}

	Matrix_2d operator%(const Matrix_2d& right) const // Hadamard product
	{
		if (right.m_rowCount != m_rowCount || right.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		auto temp = *this;
		temp %= right;
		return temp;
	}
	Matrix_2d& operator%=(const Matrix_2d& right) // Hadamard product
	{
		if (right.m_rowCount != m_rowCount || right.m_colCount != m_colCount)
			throw MatrixException("ivalid matrix");

		for (int i = 0; i < m_rowCount; i++)
		{
			for (int j = 0; j < m_colCount; j++)
				m_array[i][j] *= right.m_array[i][j];
		}
		return *this;
	}

	
	explicit operator Type() // If matrix the same as a scalar
	{
		if (1 != m_rowCount || 1 != m_colCount)
			throw MatrixException("matrix is not a scalar");

		return m_array[0][0];
	}

	std::vector<Type>& operator[](int index)
	{
		return m_array.at(index);
	}

	const std::vector<Type>& operator[](int index) const
	{
		return m_array.at(index);
	}

	const int GetCountOfRows() const
	{
		return m_rowCount;
	}

	const int GetCountOfColumns() const
	{
		return m_colCount;
	}

	void ToZero()
	{
		for (int i = 0; i < m_array.size(); i++)
			std::fill(m_array[i].begin(), m_array[i].end(), 0);
	}

	//--------------------------------------------------------------------------------------------- random

	template<class RandomEngine>
	Matrix_2d& Random(RandomEngine& rEngine)
	{
		std::for_each(m_array.begin(), m_array.end(), [&rEngine](auto& vector) {
			std::for_each(vector.begin(), vector.end(), [&rEngine](auto& element) {
				element = rEngine();
				});
			});
		return *this;
	}

	//--------------------------------------------------------------------------------------------- to see what you got

	friend std::ostream& operator<<(std::ostream& out, const Matrix_2d& matrix)
	{
		std::for_each(matrix.m_array.begin(), matrix.m_array.end(), [&out](const auto& vector) {
			out << vector << "\n";
			});
		return out;
	}

	//---------------------------------------------------------------------------------------------

	~Matrix_2d() = default;
};

#endif
