
#ifdef DEBUG
    #include <mutex>
    #include <iostream>

    class Logger{
        public:
            static std::ostream& startMessage(){return std::cout;}
            static std::mutex& getMutex(){return instance().coutMut;}
        private:
            Logger() = default;
            static Logger& instance(){static Logger logger; return logger;}
            std::mutex coutMut;
    };

    #define LOG(MESSAGE_CHAIN) \
        {\
            std::lock_guard coutlg(Logger::getMutex());\
            Logger::startMessage() << MESSAGE_CHAIN << std::endl;\
        }
#else
    #define LOG(MESSAGE_CHAIN)
#endif
