#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cstddef>
#include <cassert>
#include <cmath>

#include <type_traits>
#include <iostream>

namespace math
{
	template <size_t DIM, typename Type>
	struct Vector;

	template <typename To, size_t DIM, typename From>
	Vector<DIM, To> convert (const Vector<DIM, From> &from)
	{
		static_assert(std::is_convertible<From, To>::value, "Bad cast");

		Vector<DIM, To> to;

		for (size_t i = 0; i < DIM; ++i)
		{
			to[i] = static_cast<To>(from[i]);
		}

		return to;
	}

	/* Common Vector */
	template <size_t DIM, typename Type>
	struct Vector
	{
		Type& operator[] (size_t index) noexcept
		{
			assert(index < dim() && "Index out of range");
			return _elements[index];
		}

		const Type& operator[] (size_t index) const noexcept
		{
			assert(index < dim() && "Index out of range");
			return _elements[index];
		}

		template <typename OtherType>
		operator Vector<DIM, OtherType> () const 
		{
			return convert<OtherType>(*this);
		}

		constexpr size_t dim () const noexcept
		{
			return DIM;
		}

		Type _elements[DIM];
	};

	/* Vector2 */
	template <typename Type>
	struct Vector<2, Type>
	{
		Type& operator[] (size_t index) noexcept
		{
			assert(index < dim() && "Index out of range");
			return index == 0 ? x : y;
		}

		const Type& operator[] (size_t index) const noexcept
		{
			assert(index < dim() && "Index out of range");
			return index == 0 ? x : y;
		}

		template <typename OtherType>
		operator Vector<2, OtherType> () const
		{
			return convert<OtherType>(*this);
		}

		constexpr size_t dim() const noexcept
		{
			return 2;
		}

		Type x, y;
	};

	/* Vector3 */
	template <typename Type>
	struct Vector<3, Type>
	{
		Type& operator[] (size_t index) noexcept
		{
			assert(index < dim() && "Index out of range");
			
			switch (index)
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
			}

			return z;
		}

		const Type& operator[] (size_t index) const noexcept
		{
			assert(index < dim() && "Index out of range");
						
			switch (index)
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
			}

			return z;
		}

		template <typename OtherType>
		operator Vector<3, OtherType> () const
		{
			return convert<OtherType>(*this);
		}

		constexpr size_t dim () const noexcept
		{
			return 3;
		}

		Type x, y, z;
	};

	template <size_t DIM, typename LhvType, typename RhvType>
	auto& operator+= (Vector<DIM, LhvType> &lhv, const Vector<DIM, RhvType> &rhv)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhv[i] += rhv[i];
		}

		return lhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto operator+ (const Vector<DIM, LhvType> &lhv, const Vector<DIM, RhvType> &rhv) 
	{
		using ResType = decltype(LhvType() + RhvType());
		Vector<DIM, ResType> result(lhv);
		return result += rhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto& operator-= (Vector<DIM, LhvType> &lhv, const Vector<DIM, RhvType> &rhv)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhv[i] -= rhv[i];
		}

		return lhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto operator- (const Vector<DIM, LhvType> &lhv, const Vector<DIM, RhvType> &rhv)
	{
		using ResType = decltype(LhvType() - RhvType());
		Vector<DIM, ResType> result(lhv);
		return result -= rhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto& operator*= (Vector<DIM, LhvType> &lhv, const RhvType &rhv)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhv[i] *= rhv;
		}

		return lhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto operator* (const Vector<DIM, LhvType> &lhv, const RhvType &rhv)
	{
		using ResType = decltype(LhvType() * RhvType());
		Vector<DIM, ResType> result(lhv);
		return result *= rhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto operator* (const LhvType &lhv, const Vector<DIM, RhvType> &rhv)
	{
		return rhv * lhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	double operator* (const Vector<DIM, LhvType> &lhv, const Vector<DIM, RhvType> &rhv)
	{
		double scalar = 0;

		for (size_t i = 0; i < DIM; ++i)
		{
			scalar += static_cast<double>(lhv[i]) * rhv[i];
		}

		return scalar;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto& operator/= (Vector<DIM, LhvType> &lhv, const RhvType &rhv)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			lhv[i] /= rhv;
		}

		return lhv;
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	auto operator/ (Vector<DIM, LhvType> &lhv, const RhvType &rhv)
	{
		using ResType = decltype(LhvType() / RhvType());
		Vector<DIM, ResType> result(lhv);
		return result /= rhv;
	}

	template <size_t DIM, typename Type>
	double length (const Vector<DIM, Type> &vector)
	{
		double tmp = 0;

		for (size_t i = 0; i < DIM; ++i)
		{
			tmp += static_cast<double>(vector[i]) * vector[i]; 
		}

		return std::sqrt(tmp);
	}

	template <size_t DIM, typename LhvType, typename RhvType>
	double angle (const Vector<DIM, LhvType> &lhv, const Vector<DIM, RhvType> &rhv)
	{
		return lhv * rhv / (length(lhv) * length(rhv));
	}

	template <size_t DIM, typename Type>
	auto normalize (const Vector<DIM, Type> &vector)
	{
		Vector<DIM, double> tmp(vector);

		return tmp /= length(tmp);
	}


	/* Type alias */
	template <typename Type>
	using Vector2 = Vector<2, Type>;

	template <typename Type>
	using Vector3 = Vector<3, Type>;

	/* integer types */
	using Vector2i = Vector2<int>;
	using Vector3i = Vector3<int>;

	using Vector2u = Vector2<unsigned>;
	using Vector3u = Vector3<unsigned>;

	/* cstddef types */
	using Vector2sz = Vector2<size_t>;
	using Vector3sz = Vector3<size_t>;

	/* floating point types */
	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;

	using Vector2d = Vector2<double>;
	using Vector3d = Vector3<double>;



	template <typename LhvType, typename RhvType>
	auto cross (const Vector3<LhvType> &lhv, const Vector3<RhvType> &rhv)
	{
		using MulType = decltype(LhvType() * RhvType());
		using ResType = decltype(MulType() + MulType());

		return Vector3<ResType>
		{
			lhv[1] * rhv[2] - rhv[1] * lhv[2],
		   -lhv[0] * rhv[2] + rhv[0] * lhv[2],
			lhv[0] * rhv[1] - rhv[0] * lhv[1]
		};
	}

	template <typename FirstType, typename SecondType, typename ThirdType>
	double mixed (const Vector3<FirstType> &first, const Vector3<SecondType> &second, const Vector3<ThirdType> &third)
	{
		return first * cross(second, third);
	}

	template <size_t DIM, typename Type>
	std::istream& operator>> (std::istream &in, Vector<DIM, Type> &vector)
	{
		for (size_t i = 0; i < DIM; ++i)
		{
			in >> vector[i];	
		}

		return in;
	}

	template <size_t DIM, typename Type>
	std::ostream& operator<< (std::ostream &out, const Vector<DIM, Type> &vector)
	{
		size_t i = 0;
		for (; i < DIM - 1; ++i)
		{
			out << vector[i] << ' ';
		}
	
		return out << vector[i];
	}
}//namespace math

#endif//VECTORS_HPP