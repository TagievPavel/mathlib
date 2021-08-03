#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cstddef>
#include <cassert>
#include <cmath>

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <iterator>

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

			Vector<Size, OtherType> other;

			std::copy(std::cbegin(_elements), std::cend(_elements), std::begin(other._elements));

			return other;
		}

	public:
		constexpr size_t size () const noexcept
		{
			return Size;
		}

	public: 
		Type _elements[Size]; /* private */ 
	};

	/* Vector2 */
	template <typename Type>
	class Vector<2, Type>
	{
	public:
		Type& operator[] (size_t index) noexcept
		{
			assert(index < size() && "Index out of range");
			return index == 0 ? x : y;
		}

		const Type& operator[] (size_t index) const noexcept
		{
			assert(index < size() && "Index out of range");
			return index == 0 ? x : y;
		}

		template <typename OtherType>
		operator Vector<2, OtherType> () const
		{
			static_assert(std::is_convertible<Type, OtherType>::value, "Bad cast");

			Vector<2, OtherType> other = { x, y };

			return other;
		}

	public:
		constexpr size_t size() const noexcept
		{
			return 2;
		}

	public:
		Type x, y;
	};

	/* Vector3 */
	template <typename Type>
	class Vector<3, Type>
	{
	public:
		Type& operator[] (size_t index) noexcept
		{
			assert(index < size() && "Index out of range");
			
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
			assert(index < size() && "Index out of range");
						
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
			static_assert(std::is_convertible<Type, OtherType>::value, "Bad cast");

			Vector<3, OtherType> other = { x, y, z };

			return other;
		}

	public:
		constexpr size_t size () const noexcept
		{
			return 3;
		}

	public:
		Type x, y, z;
	};

	/* Vector4 */
	template <typename Type>
	class Vector<4, Type>
	{
	public:
		Type& operator[] (size_t index) noexcept
		{
			assert(index < size() && "Index out of range");
						
			switch (index)
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
			}

			return w;
		}

		const Type& operator[] (size_t index) const noexcept
		{
			assert(index < size() && "Index out of range");

			switch (index)
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
			}

			return w;
		}

		template <typename OtherType>
		operator Vector<4, OtherType> () const
		{
			static_assert(std::is_convertible<Type, OtherType>::value, "Bad cast");

			Vector<4, OtherType> other = { x, y, z, w };

			return other;
		}

	public:
		constexpr size_t size () const noexcept
		{
			return 4;
		}

	public:
		Type x, y, z, w;
	};

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
		return (lhv * rhv) / (length(lhv) * length(rhv));
	}

	template <size_t Size, typename Type>
	auto normalize (const Vector<Size, Type> &vector)
	{
		Vector<Size, long double> tmp(vector);

		return tmp /= length(tmp);
	}


	/* Type alias */
	template <typename Type>
	using Vector2 = Vector<2, Type>;

	template <typename Type>
	using Vector3 = Vector<3, Type>;

	template <typename Type>
	using Vector4 = Vector<4, Type>;


	/* integer types */
	using Vector2I = Vector2<int>;
	using Vector3I = Vector3<int>;
	using Vector4I = Vector4<int>;

	using Vector2LL = Vector2<long long>; 
	using Vector3LL = Vector3<long long>;
	using Vector4LL = Vector4<long long>;

	using Vector2U = Vector2<unsigned>;
	using Vector3U = Vector3<unsigned>;
	using Vector4U = Vector4<unsigned>;

	using Vector2ULL = Vector2<unsigned long long>;
	using Vector3ULL = Vector3<unsigned long long>;
	using Vector4ULL = Vector4<unsigned long long>;

	/* cstddef types */
	using Vector2SZ = Vector2<size_t>;
	using Vector3SZ = Vector3<size_t>;
	using Vector4SZ = Vector4<size_t>;

	/* floating point types */
	using Vector2F = Vector2<float>;
	using Vector3F = Vector3<float>;
	using Vector4F = Vector4<float>;

	using Vector2D = Vector2<double>;
	using Vector3D = Vector3<double>;
	using Vector4D = Vector4<double>;

	using Vector2LD = Vector2<long double>;
	using Vector3LD = Vector3<long double>;
	using Vector4LD = Vector4<long double>;


	template <typename LhvType, typename RhvType>
	auto cross (const Vector3<LhvType> &lhv, const Vector3<RhvType> &rhv)
	{
		using MultiplyType = decltype(LhvType() * RhvType());
		using ResultingType = decltype(MultiplyType() + MultiplyType());

		Vector3<ResultingType> result = 
		{
			+(lhv[1] * rhv[2] - rhv[1] * lhv[2]),
			-(lhv[0] * rhv[2] - rhv[0] * lhv[2]),
			+(lhv[0] * rhv[1] - rhv[0] * lhv[1])
		};

		return result;
	}

	template <typename FirstType, typename SecondType, typename ThirdType>
	long double mixed (const Vector3<FirstType> &first, const Vector3<SecondType> &second, const Vector3<ThirdType> &third)
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