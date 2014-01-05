/* Matrix.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * The Matrix template class provides extra functionality on top of MatrixBase.
 * It defines the Column and Row containers, and provides methods to get the
 * matrix element data within those objects.
 */

#ifndef MINER_MATRIX_H__
#define MINER_MATRIX_H__

#include "util/ContainerFacade.h"
#include "miner/MatrixBase.h"

#include <vector>

namespace Miner {

template <typename T>
class Matrix : public MatrixBase<T> {
	public:

	typedef typename MatrixBase<T>::size_type size_type;
	typedef typename MatrixBase<T>::InvalidAddressing InvalidAddressing;

	/* The column/row class:
	 *
	 * Acts as a random access container...
	 * ...but also stores the column or row number it refers to.
	 */
	template <typename U>
	class ColRow_ : public Util::RAContainerFacade<U> {
		private:
			Matrix::size_type m_Num;
		public:
			typedef typename Util::RAContainerFacade<U>::size_type size_type;

			ColRow_() : m_Num{} {}
			explicit ColRow_(size_type capacity, Matrix::size_type num = 0) :
				Util::RAContainerFacade<U>{capacity}, m_Num{num} {}
			ColRow_(ColRow_ const& rhs) : Util::RAContainerFacade<U>{rhs}, m_Num{rhs.m_Num} {}
			ColRow_(ColRow_&& rhs) : Util::RAContainerFacade<U>{rhs}, m_Num{std::move(rhs.m_Num)} {}
			ColRow_& operator=(ColRow_ const& rhs) {
				Util::RAContainerFacade<U>::operator=(rhs);
				m_Num = rhs.m_Num;
				return *this;
			}
			ColRow_& operator=(ColRow_&& rhs) {
				Util::RAContainerFacade<U>::operator=(rhs);
				m_Num = std::move(rhs.m_Num);
				return *this;
			}

			Matrix::size_type Num() const { return m_Num; }
			void Num(Matrix::size_type num) { m_Num = num; }
	};

	typedef ColRow_<T> ColRow;
	typedef ColRow Column;
	typedef ColRow Row;

	explicit Matrix(size_type squaresize) : MatrixBase<T>(squaresize) {}
	explicit Matrix(size_type columns, size_type rows) : MatrixBase<T>(columns, rows) {}
	virtual ~Matrix() {}

	Column const GetColumn(size_type colnum, size_type start = 0, size_type end = 0) const {
		this->CheckRange(start, end, this->NumRows());

		Column col(end - start, colnum);

		for (size_type j = start, n = 0; j < end; ++j, ++n) {
			col[n] = (*this)(colnum, j);
		}

		return col;
	}

	Column GetColumn(size_type colnum, size_type start = 0, size_type end = 0) {
		return const_cast<Column&&>( static_cast<Matrix<T> const&>(*this).GetColumn(colnum, start, end) );
	}

	std::vector<Column> const GetColumns(size_type cstart = 0, size_type cend = 0, size_type rstart = 0, size_type rend = 0) const {
		this->CheckRange(cstart, cend, this->NumColumns());

		std::vector<Column> columns(cend - cstart);

		for (size_type i = cstart, n = 0; i < cend; ++i, ++n) {
			columns[n] = this->GetColumn(i, rstart, rend);
		}

		return columns;
	}

	std::vector<Column> GetColumns(size_type cstart = 0, size_type cend = 0, size_type rstart = 0, size_type rend = 0) {
		return const_cast<std::vector<Column>&&>( static_cast<Matrix<T> const&>(*this).GetColumns(cstart, cend, rstart, rend) );
	}

	Row const GetRow(size_type rownum, size_type start = 0, size_type end = 0) const {
		this->CheckRange(start, end, this->NumColumns());

		Row row(end - start, rownum);

		for (size_type i = start, n = 0; i < end; ++i, ++n) {
			row[n] = (*this)(i, rownum);
		}

		return row;
	}

	Row GetRow(size_type row, size_type start = 0, size_type end = 0) {
		return const_cast<Row&&>( static_cast<Matrix<T> const&>(*this).GetRow(row, start, end) );
	}

	std::vector<Row> const GetRows(size_type rstart = 0, size_type rend = 0, size_type cstart = 0, size_type cend = 0) const {
		this->CheckRange(rstart, rend, this->NumRows());

		std::vector<Row> rows(rend - rstart);

		for (size_type i = rstart, n = 0; i < rend; ++i, ++n) {
			rows[n] = this->GetRow(i, cstart, cend);
		}

		return rows;
	}

	std::vector<Row> GetRows(size_type rstart = 0, size_type rend = 0, size_type cstart = 0, size_type cend = 0) {
		return const_cast<std::vector<Row>&&>( static_cast<Matrix<T> const&>(*this).GetRows(rstart, rend, cstart, cend) );
	}

	void ReplaceColumn(Column const& replace, size_type start = 0, size_type end = 0) {
		this->CheckRange(start, end, this->NumRows());
		size_type colnum = replace.Num();

		for (size_type j = start, n = 0; j < end; ++j, ++n) {
			(*this)(colnum, j) = replace[n];
		}
	}

	void ReplaceRow(Row const& replace, size_type start = 0, size_type end = 0) {
		this->CheckRange(start, end, this->NumColumns());
		size_type rownum = replace.Num();

		for (size_type i = start, n = 0; i < end; ++i, ++n) {
			(*this)(i, rownum) = replace[n];
		}
	}

	void SetRowValue(size_type rownum, T const& value, size_type start = 0, size_type end = 0) {
		this->CheckRange(start, end, this->NumRows());

		for (size_type i = start; i < end; ++i)
			(*this)(i, rownum) = value;
	}

	void SetColumnValue(size_type colnum, T const& value, size_type start = 0, size_type end = 0) {
		this->CheckRange(start, end, this->NumRows());

		for (size_type i = start; i < end; ++i)
			(*this)(colnum, i) = value;
	}
};

}	// Miner

#endif
