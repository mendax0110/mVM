#ifndef MACROBASE_H
#define MACROBASE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <stdexcept>


/// @brief This macro will log the exception and continue
#define LOG_EXCEPTION_AND_CONTINUE(message) \
    catch (const std::exception& e) \
    { \
        std::cerr << message << std::endl; \
        std::cerr << e.what() << std::endl; \
        std::cerr << "Exception caught at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
    catch (...) \
    { \
        std::cerr << message << std::endl; \
        std::cerr << "Exception caught at " << __FILE__ << ":" << __LINE__ << std::endl; \
    }

/// @brief This macro will log the object and continue
#define LOG_OBJECT_AND_CONTINUE(e) \
    catch (const std::exception& e) \
    { \
        std::cerr << e.obj() << std::endl; \
        std::cerr << "Exception caught at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
    catch (...) \
    { \
        std::cerr << e.obj() << std::endl; \
        std::cerr << "Exception caught at " << __FILE__ << ":" << __LINE__ << std::endl; \
    }

/// @brief This macro will log the exception and return the error code
#define LOG_EXCEPTION_AND_RETURN(message, code) \
    catch (const std::exception& e) \
    { \
        std::cerr << message << std::endl; \
        std::cerr << e.what() << std::endl; \
        std::cerr << "Exception caught at " << __FILE__ << ":" << __LINE__ << std::endl; \
        return code; \
    } \
    catch (...) \
    { \
        std::cerr << message << std::endl; \
        std::cerr << "Exception caught at " << __FILE__ << ":" << __LINE__ << std::endl; \
        return code; \
    }

#endif // MACROBASE_H
