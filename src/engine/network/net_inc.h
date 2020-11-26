#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0X0A00
#endif

#ifndef ASIO_STANDALONE 
#define ASIO_STANDALONE
#endif 

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <memory>
#include <thread>
