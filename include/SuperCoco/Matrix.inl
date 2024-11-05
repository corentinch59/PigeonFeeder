#include <SuperCoco/Maths.hpp>
#include <algorithm>
#include <cmath>
#include <cassert>

namespace Sce
{
	template<typename T>
	Matrix<T>::Matrix(uint32_t depth) :
	Matrix(depth, depth)
	{
	}

	template<typename T>
	Matrix<T>::Matrix(uint32_t depth, const std::vector<T>& list) :
	m_rows(depth),
	m_columns(depth),
	m_matrix(list)
	{
	}

	template<typename T>
	Matrix<T>::Matrix(uint32_t rows, uint32_t columns) :
	m_rows(rows),
	m_columns(columns),
	m_matrix(m_rows * m_columns, 0)
	{
	}

	template<typename T>
	Matrix<T>::Matrix(uint32_t rows, uint32_t columns, const std::vector<T>& list) :
	m_rows(rows),
	m_columns(columns),
	m_matrix(list)
	{
	}
	
	template<typename T>
	Matrix<T> Matrix<T>::operator+(const Matrix& matrix) const
	{
		Matrix newMatrix(m_rows, m_columns);
		for (size_t i = 0; i < m_rows * m_columns; i++)
		{
			newMatrix.m_matrix[i] = m_matrix[i] + matrix.m_matrix[i];
		}
		return newMatrix;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator-(const Matrix& matrix) const
	{
		Matrix newMatrix(m_rows, m_columns);
		for (size_t i = 0; i < m_rows * m_columns; i++)
		{
			newMatrix.m_matrix[i] = m_matrix[i] - matrix.m_matrix[i];
		}
		return newMatrix;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const T scalar) const
	{
		Matrix newMatrix(m_rows, m_columns);
		for (size_t i = 0; i < m_rows * m_columns; i++)
		{
			newMatrix.m_matrix[i] *= scalar;
		}
		return newMatrix;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const Matrix& matrix) const
	{
		assert(matrix.m_rows == m_columns);
		Matrix newMatrix(m_rows, matrix.m_columns);
		for (size_t i = 0; i < newMatrix.m_rows; i++)
		{
			for (size_t j = 0; j < newMatrix.m_columns; j++)
			{
				for (size_t k = 0; k < m_columns; k++)
				{
					newMatrix[Vector2i(i, j)] += operator[](Vector2i(i, k)) * matrix[Vector2i(k, j)];
				}
			}
		}
		return newMatrix;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator+=(const Matrix& matrix)
	{
		for (size_t i = 0; i < m_rows * m_columns; i++)
		{
			m_matrix[i] += matrix.m_matrix[i];
		}
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator-=(const Matrix& matrix)
	{
		for (size_t i = 0; i < m_rows * m_columns; i++)
		{
			m_matrix[i] -= matrix.m_matrix[i];
		}
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator*=(const T scalar)
	{
		for (size_t i = 0; i < m_rows * m_columns; i++)
		{
			m_matrix[i] *= scalar;
		}
		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator*=(const Matrix& matrix)
	{
		assert(matrix.m_rows == m_columns);
		Matrix newMatrix(m_rows, matrix.m_columns);
		for (size_t i = 0; i < newMatrix.m_rows; i++)
		{
			for (size_t j = 0; j < newMatrix.m_columns; j++)
			{
				for (size_t k = 0; k < m_columns; k++)
				{
					newMatrix[Vector2i(i, j)] += operator[](Vector2i(i, k)) * matrix[Vector2i(k, j)];
				}
			}
		}
		m_matrix = newMatrix.m_matrix;
		return *this;
	}

	template<typename T>
	Vector2<T> Matrix<T>::operator*(const Vector2<T>& vec) const
	{
		Vector2<T> result;
		result.x = operator[](Vector2i{ 0, 0 }) * vec.x + operator[](Vector2i{ 0, 1 }) * vec.y + operator[](Vector2i{ 0, 2 });
		result.y = operator[](Vector2i{ 1, 0 }) * vec.x + operator[](Vector2i{ 1, 1 }) * vec.y + operator[](Vector2i{ 1, 2 });

		return result;
	}

	template<typename T>
	T Matrix<T>::operator[](Vector2i coords) const
	{
		assert(coords.x <= m_rows);
		assert(coords.y <= m_columns);
		return m_matrix[coords.y + (m_columns * coords.x)];
	}

	template<typename T>
	T& Matrix<T>::operator[](Vector2i coords)
	{
		assert(coords.x <= m_rows);
		assert(coords.y <= m_columns);
		return m_matrix[coords.y + (m_columns * coords.x)];
	}

	template<typename T>
	std::string Matrix<T>::ToString() const
	{
		std::string msg;
		for (size_t i = 0; i < m_rows; i++)
		{
			msg.append("| ");
			for (size_t j = 0; j < m_columns; j++)
			{
				msg.append(std::to_string(operator[](Vector2i(i, j))));
				msg.append(" ");
			}
			msg.append(" |\n");
		}
		return msg;
	}

	template<typename T>
	uint32_t Matrix<T>::GetNbRows() const
	{
		return m_rows;
	}

	template<typename T>
	uint32_t Matrix<T>::GetNbColumns() const
	{
		return m_columns;
	}

	template<typename T>
	Vector2<T> Matrix<T>::GetVector2() const
	{
		assert(m_rows == 3);
		assert(m_columns == 3);
		return Vector2<T>(operator[](Vector2i(0, 2)), operator[](Vector2i(1, 2)));
	}

	template<typename T>
	Matrix<T> Matrix<T>::GetIdentity()
	{
		assert(m_rows == m_columns);
		Matrix newMatrix(m_rows);
		for (size_t i = 0; i < m_rows; i++)
		{
			newMatrix[(Vector2i(i, i))] = static_cast<T>(1);
		}
		return newMatrix;
	}

	template<typename T>
	Matrix<T> Matrix<T>::Split(uint32_t column)
	{
		Matrix smatrix1(m_rows, column + 1);
		Matrix smatrix2(m_rows, m_columns - column - 1);

		for (size_t i = 0; i < m_rows; i++)
		{
			for (size_t j = 0; j < smatrix1.m_columns; j++)
			{
				smatrix1[Vector2i(i, j)] = operator[](Vector2i(i, j));
			}

			for (size_t j = smatrix1.m_columns; j < m_columns; j++)
			{
				smatrix2[Vector2i(i, j - smatrix1.m_columns)] = operator[](Vector2i(i, j));
			}
		}

		m_matrix = smatrix1.m_matrix;
		m_columns = smatrix1.m_columns;
		m_rows = smatrix1.m_rows;

		return smatrix2;
	}

	template<typename T>
	Matrix<T> Matrix<T>::GetAugmentedMatrix(const Matrix& matrix)
	{
		assert(m_rows == matrix.m_rows);
		Matrix tempMatrix(m_rows, m_columns + matrix.m_columns);

		for (int i = 0; i < tempMatrix.m_rows; ++i)
		{
			for (int j = 0; j < m_columns; ++j)
			{
				tempMatrix[Vector2i(i, j)] = operator[](Vector2i(i, j));
			}

			for (int j = 0; j < matrix.m_columns; ++j)
			{
				tempMatrix[Vector2i(i, j + m_columns)] = matrix[Vector2i(i, j)];
			}
		}

		return tempMatrix;
	}

	template<typename T>
	Matrix<T> Matrix<T>::InvertByRowReduction()
	{
		assert(m_rows == m_columns);
		Matrix agmatrix = GetAugmentedMatrix(GetIdentity());

		auto FindBigger = [&](uint32_t startingLine, uint32_t column)
		{
			uint32_t result = startingLine;
			T temp = operator[](Vector2i(startingLine, column));
			for (size_t i = startingLine; i < agmatrix.m_rows; ++i)
			{
				if (agmatrix[Vector2i(i, column)] > temp && agmatrix[Vector2i(i, column)] != 0)
				{
					result = i;
					temp = agmatrix[Vector2i(i, column)];
				}
			}

			return result;
		};

		for (size_t i = 0; i < m_rows; ++i)
		{
			uint32_t biggerColumn = FindBigger(i, i);
			if (agmatrix[Vector2i(i, biggerColumn)] == 0)
			{
				break;
			}

			agmatrix.SwapLines(i, biggerColumn);
			agmatrix.MultiplyLine(i, 1 / agmatrix[Vector2i(i, i)]);
			for (size_t j = 0; j < m_rows; ++j)
			{
				if (j == i)
					continue;

				agmatrix.AddLineToAnother(i, j, -agmatrix[Vector2i(j, i)]);
			}
		}

		return agmatrix;
	}

	template<typename T>
	void Matrix<T>::SwapLines(uint32_t line1, uint32_t line2)
	{
		assert(line1 < m_rows);
		assert(line2 < m_rows);
		for (size_t i = 0; i < m_columns; i++)
		{
			std::swap(operator[](Vector2i(line1, i)), operator[](Vector2i(line2, i)));
		}
	}

	template<typename T>
	void Matrix<T>::SwapColumns(uint32_t column1, uint32_t column2)
	{
		assert(column1 < m_columns);
		assert(column2 < m_columns);
		for (size_t i = 0; i < m_rows; i++)
		{
			std::swap(operator[](Vector2i(i, column1)), operator[](Vector2i(i, column2)));
		}
	}

	template<typename T>
	void Matrix<T>::MultiplyLine(uint32_t line, T scalar)
	{
		assert(scalar != 0);
		for (size_t i = 0; i < m_columns; i++)
		{
			operator[](Vector2i(line, i)) *= scalar;
		}
	}

	template<typename T>
	void Matrix<T>::MultiplyColumn(uint32_t column, T scalar)
	{
		assert(scalar != 0);
		for (size_t i = 0; i < m_rows; i++)
		{
			operator[](Vector2i(i, column)) *= scalar;
		}
	}

	template<typename T>
	void Matrix<T>::AddLineToAnother(uint32_t line1, uint32_t line2, T scalar)
	{
		for (size_t i = 0; i < m_columns; i++)
		{
			operator[](Vector2i(line2, i)) += operator[](Vector2i(line1, i)) * scalar;
		}
	}

	template<typename T>
	void Matrix<T>::AddColumnToAnother(uint32_t column1, uint32_t column2, T scalar)
	{
		for (size_t i = 0; i < m_rows; i++)
		{
			operator[](Vector2i(i, column2)) += operator[](Vector2i(i, column1)) * scalar;
		}
	}

	template<typename T>
	Matrix<T> Matrix<T>::Identity(uint32_t depth)
	{
		Matrix idmatrix(depth);
		for (size_t i = 0; i < depth; i++)
		{
			idmatrix[Vector2i(i, i)] = static_cast<T>(1);
		}
		return idmatrix;
	}

	template<typename T>
	Matrix<T> Matrix<T>::MakeFromPosition(const Vector2<T>& position)
	{
		return Matrix(3, std::vector<T>{
			1, 0, position.x,
			0, 1, position.y,
			0, 0, 1});
	}

	template<typename T>
	Matrix<T> Matrix<T>::MakeTransform3x3(const Vector2<T>& position, float rotation, const Vector2<T>& scale)
	{
		Matrixf positionMatrix{3, std::vector<float>{1, 0, position.x, 0, 1,  position.y, 0, 0, 1} };
		Matrixf rotationMatrix{3, std::vector<float>{static_cast<float>(std::cos(Deg2Rad * rotation)), static_cast<float>(std::sin(Deg2Rad * rotation) * -1), 0, static_cast<float>(std::sin(Deg2Rad * rotation)), static_cast<float>(std::cos(Deg2Rad * rotation)), 0, 0, 0, 1} };
		Matrixf scaleMatrix{ 3, std::vector<float>{scale.x, 0, 0, 0, scale.y, 0, 0, 0, 1} };

		return positionMatrix * rotationMatrix * scaleMatrix;
	}

}