/* MatrixBase.h - Copyright (c) 2014 Alejandro Martinez Ruiz
 *
 * A template class describing a matrix of elements.
 */

#ifndef MINER_MATRIXBASE_H__
#define MINER_MATRIXBASE_H__

#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>

namespace Miner {

template <typename T>
class MatrixBase {
	public:
		// basic iterator support types
		typedef T value_type;
		typedef value_type* iterator;
		typedef value_type const* const_iterator;
		typedef unsigned int size_type;

		// in C++11, 2+ args should do well to use explicit too
		explicit MatrixBase(size_type columns, size_type rows) :
				m_Columns {columns},
				m_Rows {rows},
				m_Size {m_Columns * m_Rows},
				m_Data {} {
			Init();
		}

		explicit MatrixBase(size_type squaresize) : MatrixBase(squaresize, squaresize) {}

		// copy constructor
		MatrixBase(MatrixBase const& rhs) : m_Columns{rhs.m_Columns}, m_Rows{rhs.m_Rows},
				m_Size{rhs.m_Size}, m_Data{new T[m_Size]} {
			try {
				std::copy(rhs.cbegin(), rhs.cend(), &m_Data[0]);
			} catch (std::exception &e) {
				delete[] m_Data;
				throw;
			}
		}

		// move constructor
		MatrixBase(MatrixBase&& rhs) : m_Columns{std::move(rhs.m_Columns)},
				m_Rows{std::move(rhs.m_Rows)}, m_Size{std::move(rhs.m_Size)},
				m_Data{std::move(rhs.m_Data)} {
			rhs.m_Data = nullptr;
		}

		// assignment operator implemented by swapping buffers
		MatrixBase& operator=(MatrixBase const& rhs) {
			if (this == &rhs)
				return *this;

			T *copy = new T[rhs.m_Size];

			try {
				std::copy(rhs.cbegin(), rhs.cend(), &copy[0]);
			} catch (std::exception &e) {
				delete[] copy;
				throw;
			}

			m_Columns = rhs.m_Columns;
			m_Rows = rhs.m_Rows;
			m_Size = m_Columns * m_Rows;
			delete[] m_Data;
			m_Data = copy;

			return *this;
		}

		// move assignment operator
		MatrixBase& operator=(MatrixBase&& rhs) {
			m_Columns = std::move(rhs.m_Columns);
			m_Rows = std::move(rhs.m_Rows);
			m_Size = std::move(rhs.m_Size);
			m_Data = std::move(rhs.m_Data);
			rhs.m_Data = nullptr;

			return *this;
		}

		virtual ~MatrixBase() noexcept { delete[] m_Data; }

		// exceptions thrown
		struct InvalidAddressing : public std::out_of_range {
			explicit InvalidAddressing(std::string const& s = "Invalid addressing") :
				std::out_of_range(s) {}
		};
		struct InvalidRow : public InvalidAddressing {
			explicit InvalidRow(std::string const& s = "Invalid row") :
				InvalidAddressing(s) {}
		};
		struct InvalidColumn : public InvalidAddressing {
			explicit InvalidColumn(std::string const& s = "Invalid column") :
				InvalidAddressing(s) {}
		};

		// convenience methods encapsulating attributes
		size_type NumColumns() const { return m_Columns; }
		size_type NumRows() const { return m_Rows; }
		size_type Size() const { return m_Size; }

		// provide basic iterators
		iterator begin() noexcept { return &m_Data[0]; }
		iterator end() noexcept { return &m_Data[this->Size()]; }
		const_iterator cbegin() const noexcept { return &const_cast<value_type const *>(m_Data)[0]; }
		const_iterator cend() const noexcept { return &const_cast<value_type const *>(m_Data)[this->Size()]; }

		// crange() can be used in C++11 for-ranges as a const range
		MatrixBase const& crange() const noexcept { return *this; }

		// addressing single T elements
		T const& operator()(size_type column, size_type row) const {
			// test for correct addressing
			if (row >= m_Rows)
				throw InvalidRow();
			if (column >= m_Columns)
				throw InvalidColumn();

			return m_Data[m_Columns * row + column];
		}

		T& operator()(size_type column, size_type row) {
			return const_cast<T&>( static_cast<MatrixBase<T> const&>(*this)(column, row) );
		}

		// these two below are synonyms to operator()
		T const& At(size_type column, size_type row) const {
			return (*this)(column, row);
		}

		T& At(size_type column, size_type row) {
			return const_cast<T&>( static_cast<MatrixBase<T> const&>(*this)(column, row) );
		}

		/* The methods below perform operations on T elements in columns and rows.
		 *
		 * Another strategy to perform those tasks is writing different iterator classes.
		 */
		void EachInColumn(size_type col, std::function<void(size_type, size_type, T&)> const& func, size_type start = 0, size_type end = 0) {
			CheckRange(start, end, this->NumRows());

			for (size_type row = start; row < end; ++row) {
				func(col, row, (*this)(col, row));
			}
		}

		void EachInRow(size_type row, std::function<void(size_type, size_type, T&)> const& func, size_type start = 0, size_type end = 0) {
			CheckRange(start, end, this->NumColumns());

			for (size_type col = start; col < end; ++col) {
				func(col, row, (*this)(col, row));
			}
		}

		// swap two T elements using coordinates
		void Swap(size_type col1, size_type row1, size_type col2, size_type row2) {
			T tmp { (*this)(col1, row1) };
			(*this)(col1, row1) = (*this)(col2, row2);
			(*this)(col2, row2) = tmp;
		}

	protected:

		// throw if invalid coordinates are given - modifies in-place "end" argument
		static void CheckRange(size_type& start, size_type& end, size_type bounds) {
			if (end == 0)
				end = bounds;
			if (end > bounds || end <= start)
				throw InvalidAddressing();
		}

	private:

		void Init() {
			// refuse to build a useless matrix
			if (m_Columns == 0 || m_Rows == 0)
				throw InvalidAddressing();

			m_Data = new T[m_Size];
		}

		size_type m_Columns, m_Rows, m_Size;
		T* m_Data;
};

}	// Miner

#endif
