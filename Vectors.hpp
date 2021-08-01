#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cstddef>
#include <cassert>
#include <cmath>

#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <iostream>

namespace math
{
	/* Common Vector */
	template <size_t Size, typename Type>
	class Vector
	{
	public:
		Type& operator[] (size_t index) noexcept
		{
			assert(index < size() && "Index out of range");
			return _elements[index];
		}

		const Type& operator[] (size_t index) const noexcept
		{
			assert(index < size() && "Index out of range");
			return _elements[index];
		}

		template <typename OtherType>
		operator Vector<Size, OtherType> () const 
		{
			static_assert(std::is_convertible<Type, OtherType>::value, "Bad cast");

			Vector<Size, OtherType> result;

			for (size_t i = 0; i < Size; ++i)
			{
				result[i] = _elements[i];
			}

			return result;
		}

	public:
		constexpr size_t size () const noexcept
		{
			return Size;
		}

	public: 
		Type _elements[Size]; /* private */ 
	};

	//TODO:
	/* Vector2 */
	/* Vector3 */
	/* Vector4 */

	template <size_t Size, typename LhvType, typename RhvType>
	auto& operator+= (Vector<Size, LhvType> &lhv, const Vector<Size, RhvType> &rhv)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			lhv[i] += rhv[i];
		}

		return lhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto operator+ (const Vector<Size, LhvType> &lhv, const Vector<Size, RhvType> &rhv) 
	{
		Vector<Size, decltype(LhvType() + RhvType())> result(lhv);

		return result += rhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto& operator-= (Vector<Size, LhvType> &lhv, const Vector<Size, RhvType> &rhv)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			lhv[i] -= rhv[i];
		}

		return lhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto operator- (const Vector<Size, LhvType> &lhv, const Vector<Size, RhvType> &rhv)
	{
		Vector<Size, decltype(LhvType() + RhvType())> result(lhv);

		return result -= rhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto& operator*= (Vector<Size, LhvType> &lhv, const RhvType &rhv)
	{
		for (size_t i = 0; i < lhv.size(); ++i)
		{
			lhv[i] *= rhv;
		}

		return lhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto operator* (const Vector<Size, LhvType> &lhv, const RhvType &rhv)
	{
		Vector<Size, decltype(LhvType() * RhvType())> result(lhv);

		return result *= rhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto operator* (const LhvType &lhv, const Vector<Size, RhvType> &rhv)
	{
		return rhv * lhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	long double operator* (const Vector<Size, LhvType> &lhv, const Vector<Size, RhvType> &rhv)
	{
		long double scalar = 0;

		for (size_t i = 0; i < Size; ++i)
		{
			scalar += static_cast<long double>(lhv[i]) * rhv[i];
		}

		return scalar;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto& operator/= (Vector<Size, LhvType> &lhv, const RhvType &rhv)
	{
		for (size_t i = 0; i < lhv.size(); ++i)
		{
			lhv[i] /= rhv;
		}

		return lhv;
	}

	template <size_t Size, typename LhvType, typename RhvType>
	auto operator/ (Vector<Size, LhvType> &lhv, const RhvType &rhv)
	{
		Vector<Size, decltype(LhvType() / RhvType())> result(lhv);

		return result /= rhv;
	}

	template <size_t Size, typename Type>
	long double length (const Vector<Size, Type> &vector)
	{
		long double tmp = 0;

		for (size_t i = 0; i < vector.size(); ++i)
		{
			tmp += static_cast<long double>(vector[i]) * vector[i]; 
		}

		return std::sqrtl(tmp);
	}

	template <size_t Size, typename LhvType, typename RhvType>
	long double angle (const Vector<Size, LhvType> &lhv, const Vector<Size, RhvType> &rhv)
	{
		return (rhv * lhv) / (length(lhv) * length(rhv));
	}

	template <size_t Size, typename Type>
	auto normalize (const Vector<Size, Type> &vector)
	{
		Vector<Size, long double> tmp(vector);

		return tmp /= length(tmp);
	}

	template <typename LhvType, typename RhvType>
	auto cross (const Vector<3, LhvType> &lhv, const Vector<3, RhvType> &rhv)
	{
		using MultiplyType = decltype(LhvType() * RhvType());
		using ResultingType = decltype(MultiplyType() + MultiplyType());

		Vector<3, ResultingType> result = 
		{
			+(lhv[1] * rhv[2] - rhv[1] * lhv[2]),
			-(lhv[0] * rhv[2] - rhv[0] * lhv[2]),
			+(lhv[0] * rhv[1] - rhv[0] * lhv[1])
		};

		return result;
	}

	template <typename FirstType, typename SecondType, typename ThirdType>
	long double mixed (const Vector<3, FirstType> &first, const Vector<3, SecondType> &second, const Vector<3, ThirdType> &third)
	{
		return first * cross(second, third);
	}

	template <size_t Size, typename Type>
	std::istream& operator>> (std::istream &in, Vector<Size, Type> &vector)
	{
		for (size_t i = 0; i < vector.size(); ++i)
		{
			in >> vector[i];	
		}

		return in;
	}

	template <size_t Size, typename Type>
	std::ostream& operator<< (std::ostream &out, const Vector<Size, Type> &vector)
	{
		size_t i = 0;
		for (; i < vector.size() - 1; ++i)
		{
			out << vector[i] << ' ';
		}
	
		return out << vector[i];
	}
}//namespace math

#endif//VECTORS_HPP