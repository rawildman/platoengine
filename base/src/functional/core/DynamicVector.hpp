#ifndef PLATO_FUNCTIONAL_CORE_DYNAMICVECTOR
#define PLATO_FUNCTIONAL_CORE_DYNAMICVECTOR

#include <valarray>
#include <vector>

namespace Plato::Functional::Core
{
template <typename T>
class DynamicVector;

template <typename Archive, typename U>
void serialize(Archive& aArchive, DynamicVector<U>& aVector, const unsigned int aVersion);

/// @brief A wrapper for a valarray, with functions for addition, multiplication and serialization.
///
/// The purpose of this class is to provide an interface with ROL vector types, isolating the ROL types
/// to the objective/constraint ROL classes.
/// Serialization is also implemented for communication using boost::mpi.
template <typename T>
class DynamicVector
{
   public:
    DynamicVector() = default;
    explicit DynamicVector(const std::vector<T>& aVector);

    std::size_t size() const;
    const T& operator[](std::size_t aIndex) const;

    /// @pre The size of @a aRightOperand must be equal to this object's size. Checked with an assertion.
    DynamicVector<T>& operator+=(const DynamicVector<T>& aRightOperand);
    DynamicVector<T>& operator*=(const T aScalar);

    template <typename Archive, typename U>
    friend void serialize(Archive& aArchive, DynamicVector<U>& aVector, const unsigned int aVersion);

   private:
    std::valarray<T> mVector;
};

template <typename T>
DynamicVector<T> operator+(DynamicVector<T>&& aLeft, const DynamicVector<T>& aRight)
{
    aLeft += aRight;
    return std::move(aLeft);
}

template <typename T>
DynamicVector<T> operator+(const DynamicVector<T>& aLeft, DynamicVector<T>&& aRight)
{
    aRight += aLeft;
    return std::move(aRight);
}

template <typename T>
DynamicVector<T> operator+(DynamicVector<T>&& aLeft, DynamicVector<T>&& aRight)
{
    aLeft += aRight;
    return std::move(aLeft);
}

template <typename T>
DynamicVector<T> operator+(const DynamicVector<T>& aLeft, const DynamicVector<T>& aRight)
{
    DynamicVector<T> tCopy = aLeft;
    tCopy += aRight;
    return tCopy;
}

template <typename T>
DynamicVector<T> operator*(DynamicVector<T> aLeft, const T& aRight)
{
    aLeft *= aRight;
    return aLeft;
}

template <typename T>
DynamicVector<T> operator*(const T& aLeft, DynamicVector<T> aRight)
{
    aRight *= aLeft;
    return aRight;
}

template <typename T>
DynamicVector<T>::DynamicVector(const std::vector<T>& aVector) : mVector(aVector.data(), aVector.size())
{
}

template <typename T>
std::size_t DynamicVector<T>::size() const
{
    return mVector.size();
}

template <typename T>
const T& DynamicVector<T>::operator[](std::size_t aIndex) const
{
    return mVector[aIndex];
}

template <typename T>
DynamicVector<T>& DynamicVector<T>::operator+=(const DynamicVector<T>& aRightOperand)
{
    assert(size() == aRightOperand.size());
    mVector += aRightOperand.mVector;
    return *this;
}

template <typename T>
DynamicVector<T>& DynamicVector<T>::operator*=(const T aScalar)
{
    mVector *= aScalar;
    return *this;
}

}  // namespace Plato::Functional::Core

#endif
