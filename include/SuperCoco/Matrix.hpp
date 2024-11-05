#pragma once

#ifndef SUPER_COCO_MATRIX_HPP
#define SUPER_COCO_MATRIX_HPP

#include <SuperCoco/Vector2.hpp>
#include <vector>
#include <string>

namespace Sce
{
	template<typename T>
	class Matrix
	{
		public:
			Matrix() = delete;
			Matrix(uint32_t depth);
			Matrix(uint32_t depth, const std::vector<T>& list);
			Matrix(uint32_t rows, uint32_t columns);
			Matrix(uint32_t rows, uint32_t columns, const std::vector<T>& list);

			Matrix operator+(const Matrix& matrix) const;
			Matrix operator-(const Matrix& matrix) const;
			Matrix operator*(const T scalar) const;
			Matrix operator*(const Matrix& matrix) const;

			Vector2<T> operator*(const Vector2<T>& vec) const;

			Matrix& operator+=(const Matrix& matrix);
			Matrix& operator-=(const Matrix& matrix);
			Matrix& operator*=(const T scalar);
			Matrix& operator*=(const Matrix& matrix);

			T operator[](Vector2i coords) const;
			T& operator[](Vector2i coords);

			std::string ToString() const;
			uint32_t GetNbRows() const;
			uint32_t GetNbColumns() const;
			Vector2<T> GetVector2() const;

			Matrix GetIdentity();
			Matrix Split(uint32_t column);
			Matrix GetAugmentedMatrix(const Matrix& matrix);
			Matrix InvertByRowReduction();

			void SwapLines(uint32_t line1, uint32_t line2);
			void SwapColumns(uint32_t column1, uint32_t column2);
			void MultiplyLine(uint32_t line, T scalar);
			void MultiplyColumn(uint32_t column, T scalar);
			void AddLineToAnother(uint32_t line1, uint32_t line2, T scalar);
			void AddColumnToAnother(uint32_t column1, uint32_t column2, T scalar);

			static Matrix Identity(uint32_t depth);
			static Matrix MakeFromPosition(const Vector2<T>& position);
			static Matrix MakeTransform3x3(const Vector2<T>& position, float rotation, const Vector2<T>& scale);

		private:
			uint32_t m_rows;
			uint32_t m_columns;
			std::vector<T> m_matrix;
	};

	using Matrixf = Matrix<float>;
	using Matrixi = Matrix<int>;
}

#include <SuperCoco/Matrix.inl>

#endif