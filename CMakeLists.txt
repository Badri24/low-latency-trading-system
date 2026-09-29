cmake_minimum_required(VERSION 3.22)

project(low_latency_trading_system
    VERSION 0.1
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 17)  # C++ 17 
set(CMAKE_CXX_STANDARD_REQUIRED ON) # don't silently compile with an older C++ standard.
set(CMAKE_CXX_EXTENSIONS OFF)       # use standard C++, rather than compiler-specific GNU extensions.

# ============================================================
# Order Book Library
# ============================================================

add_library(order_book
    src/order_book.cpp
)

target_include_directories(order_book
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# ============================================================
# Market feed Library
# ============================================================

add_library(market_feed_simulator
    src/market_feed_simulator.cpp
)

target_include_directories(market_feed_simulator
   PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

# ============================================================
# event processor Library
# ============================================================
add_library(event_processor 
   src/event_processor.cpp 
)

target_include_directories(event_processor
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include    
)

target_link_libraries(event_processor
   PUBLIC
        order_book
)

# ============================================================
# Main Trading System Application
# ============================================================

add_executable(trading_system 
    src/main.cpp
    ) 
    
target_link_libraries(trading_system
    PRIVATE
    order_book
)

# ============================================================
# Learning examples
# ============================================================

add_executable(example_add
    examples/01_add_example.cpp 
    )

target_link_libraries(example_add
    PRIVATE
    order_book
)

add_executable(example_best_bid_ask
    examples/02_best_bid_ask.cpp
)

target_link_libraries(example_best_bid_ask
    PRIVATE
        order_book
)


add_executable(example_cancel
    examples/03_cancel_example.cpp
)

target_link_libraries(example_cancel
    PRIVATE
        order_book
)


add_executable(example_modify_quantity
    examples/04_modify_quantity.cpp
)

target_link_libraries(example_modify_quantity
    PRIVATE
        order_book
)

add_executable(example_modify_price
    examples/05_modify_price.cpp
)

target_link_libraries(example_modify_price
    PRIVATE
        order_book
)

add_executable(example_trade
   examples/06_trade_example.cpp
)

target_link_libraries(example_trade
    PRIVATE
        order_book
)

add_executable(example_market_feed_simulator
   examples/07_market_feed_simulator.cpp
)

target_link_libraries(example_market_feed_simulator
    PRIVATE
        market_feed_simulator
)

add_executable(example_event_processor
   examples/08_event_processor.cpp
) 

target_link_libraries(example_event_processor
    PRIVATE
        market_feed_simulator
        event_processor
)
