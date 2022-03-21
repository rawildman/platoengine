/*
 * XMLGeneratorOperationsDataStructures.hpp
 *
 *  Created on: March 21, 2022
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace XMLGen
{

/******************************************************************************//**
 * \struct OperationArgument
 * \brief Associative container of operation input and output arguments.
**********************************************************************************/
struct OperationArgument
{
private:
    std::unordered_map<std::string, std::string> mData; /*!< associative container */

public:
    OperationArgument(){}

    /******************************************************************************//**
     * \fn set
     * \brief Set value associated to a given key in the associative container.
     * \param [in] aKey   input key
     * \param [in] aValue key value
    **********************************************************************************/
    void set(const std::string& aKey, const std::string& aValue);

    /******************************************************************************//**
     * \fn get
     * \brief Get value associated to key in associative container.
     * \param [in] aKey   input key
     * \param [in] aValue key value
     * \return const reference to a string
    **********************************************************************************/
    const std::string& get(const std::string& aKey) const;
};
// struct OperationArgument

/******************************************************************************//**
 * \struct OperationMetaData
 * \brief Associative container of operation metadata.
**********************************************************************************/
struct OperationMetaData
{
private:
    std::unordered_map<std::string, std::vector<std::string>> mData; /*!< associative container */

public:
    OperationMetaData(){}
    
    /******************************************************************************//**
     * \fn append
     * \brief Append value associated to key into associative container.
     * \param [in] aKey   input key
     * \param [in] aValue key value
    **********************************************************************************/
    void append(const std::string& aKey, const std::string& aValue);

    /******************************************************************************//**
     * \fn set
     * \brief Set list associated to a given key in the associative container.
     * \param [in] aKey   input key
     * \param [in] aValue key value
    **********************************************************************************/
    void set(const std::string& aKey, const std::vector<std::string>& aValues);

    /******************************************************************************//**
     * \fn get
     * \brief Set list associated to a given key in the associative container.
     * \param [in] aKey   input key
     * \param [in] aValue key value
     * \return const reference to a list of strings
    **********************************************************************************/
    const std::vector<std::string>& get(const std::string& aKey) const;

    /******************************************************************************//**
     * \fn is_defined
     * \brief Check if key is defined in the associative container.
     * \param [in] aKey   input key
     * \return boolean (true or false)
    **********************************************************************************/
    bool is_defined(const std::string& aKey) const;

    /******************************************************************************//**
     * \fn front
     * \brief Return constant reference to the first element in the vector container associated with the input key.
     * \param [in] aKey input key
     * \return constant reference to a string
    **********************************************************************************/
    const std::string& front(const std::string& aKey) const;

    /******************************************************************************//**
     * \fn back
     * \brief Return constant reference to the last element in the vector container associated with the input key.
     * \param [in] aKey input key
     * \return constant reference to a string
    **********************************************************************************/
    std::string back(const std::string& aKey) const;

    /******************************************************************************//**
     * \fn keys
     * \brief Return vector container of all the keys defined in the associative container.
     * \return vector container
    **********************************************************************************/
    std::vector<std::string> keys() const;
};
// struct OperationMetaData

}
// namespace XMLGen