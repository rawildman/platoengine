#pragma once

#include "Plato_Utils.hpp"

#include <boost/config.hpp>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <fstream>

namespace Plato{
/// Strongly-typed string for holding an XML file name
using XMLFileName = Utils::NamedType<std::string, struct XMLFileNameTag>;
/// Strongly-typed string for holding an XML node name
using XMLNodeName = Utils::NamedType<std::string, struct XMLNodeNameTag>;

template<typename T>
void saveToXML(const T& aT, const XMLFileName& aFileName, const XMLNodeName& aNodeName)
{
    std::ofstream tOutFileStream(aFileName.mValue.c_str());
    boost::archive::xml_oarchive tOutputArchive(tOutFileStream, boost::archive::no_header | boost::archive::no_tracking);
    tOutputArchive << boost::serialization::make_nvp(aNodeName.mValue.c_str(), aT); 
}

template<typename T>
void loadFromXML(T& aT, const XMLFileName& aFileName, const XMLNodeName& aNodeName)
{
    std::ifstream tInFileStream(aFileName.mValue.c_str());
    constexpr int kNoHeader = 1;
    boost::archive::xml_iarchive tInputArchive(tInFileStream, kNoHeader);
    tInputArchive >> boost::serialization::make_nvp(aNodeName.mValue.c_str(), aT);
}
}

#define LOAD_ARCHIVE(  ) 				                                    \
	void load(std::string aFilename)                                        \
    {                                                                       \
        std::ifstream tInFileStream(aFilename.c_str());                     \
        boost::archive::text_iarchive tInputArchive(tInFileStream,4);       \
        tInputArchive >> BOOST_SERIALIZATION_NVP(*this);                    \
        tInFileStream.close();                                              \
    }                                           

#define SAVE_ARCHIVE(  ) 				                                    \
	void save(std::string aFilename)                                        \
    {                                                                       \
        std::ofstream tOutFileStream(aFilename.c_str());                    \
        boost::archive::text_oarchive tOutputArchive(tOutFileStream);       \
        tOutputArchive << BOOST_SERIALIZATION_NVP(*this);                   \
        tOutFileStream.close();                                             \
    }                                           


#define LOAD_ARCHIVE_XML(  ) 				                                \
	void loadXML(std::string aFilename)                                     \
    {                                                                       \
        std::ifstream tInFileStream(aFilename.c_str());                     \
        boost::archive::xml_iarchive tInputArchive(tInFileStream,4);        \
        tInputArchive >> BOOST_SERIALIZATION_NVP(*this);                    \
        tInFileStream.close();                                              \
    }                                           

#define SAVE_ARCHIVE_XML(  ) 				                                \
	void saveXML(std::string aFilename)                                     \
    {                                                                       \
        std::ofstream tOutFileStream(aFilename.c_str());                    \
        boost::archive::xml_oarchive tOutputArchive(tOutFileStream);        \
        tOutputArchive << BOOST_SERIALIZATION_NVP(*this);                   \
        tOutFileStream.close();                                             \
    }   

#define ARCHIVE_IO() LOAD_ARCHIVE() SAVE_ARCHIVE() LOAD_ARCHIVE_XML() SAVE_ARCHIVE_XML()