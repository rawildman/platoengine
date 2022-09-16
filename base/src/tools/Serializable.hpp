#pragma once

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
template<typename T>
void saveToXML(const T& aT, const std::string& aTopNodeName, const std::string& aFileName)
{
    std::ofstream tOutFileStream(aFileName.c_str());
    boost::archive::xml_oarchive tOutputArchive(tOutFileStream, boost::archive::no_header | boost::archive::no_tracking);
    tOutputArchive << boost::serialization::make_nvp(aTopNodeName.c_str(), aT); 
}

template<typename T>
void loadFromXML(T& aT, const std::string& aTopNodeName, const std::string& aFileName)
{
    std::ifstream tInFileStream(aFileName.c_str());
    constexpr int kNoHeader = 1;
    boost::archive::xml_iarchive tInputArchive(tInFileStream, kNoHeader);
    tInputArchive >> boost::serialization::make_nvp(aTopNodeName.c_str(), aT);
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