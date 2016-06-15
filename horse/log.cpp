#include "log.hpp"

clog& clog::instance()
{
	static clog log;
	return log;
}

void clog::init(std::string& app, bool withcout)
{
	withcout_ = withcout;
	/*!
	 * \li %y, %Y, %m, %d - date components, in Boost.DateTime meaning.
	 * \li %H, %M, %S, %f - time components, in Boost.DateTime meaning.
	 * \li %N - numeric file counter. May also contain width specification
	 *     in printf-compatible form (e.g. %5N). The resulting number will always be zero-filled.
	 * \li %% - a percent sign
	*/
	app += "_%Y%m%d_%H%M%S.log";
	logging::add_file_log
		(
		keywords::file_name = app,						                                     /*< file name pattern >*/
		//keywords::rotation_size = 10 * 1024,						                         /*< rotate files every 10 MiB... >*/
// 		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 1),		 /*< ...or at midnight >*/
		keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::hours(1)),
		keywords::format = "[%TimeStamp%]: %Message%"                                        /*< clog record format >*/
		);

// 	logging::core::get()->set_filter
// 		(
// 		logging::trivial::severity >= logging::trivial::trace
// 		);

	logging::add_common_attributes();
}

clog& clog::operator << (log_level lv)
{
	lv_ = lv;
	return *this;
}