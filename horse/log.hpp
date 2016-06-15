#pragma once
#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace logging::trivial;

class clog
{
public:
	enum log_level
	{
		trace,
		debug,
		info,
		warning,
		error,
		fatal
	};
	static clog& instance();
	void init(std::string&, bool);
	clog& operator << (log_level lv);

	template<class T>
	clog& operator << (T t)
	{
		if(withcout_) std::cout<<t<<std::endl;

		BOOST_LOG_SEV(lg_, severity_level(lv_)) << t;
		return *this;
	}
private:
	src::severity_logger< severity_level > lg_;
	log_level lv_;
	bool withcout_;
};