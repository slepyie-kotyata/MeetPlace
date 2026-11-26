if(
    NOT TARGET mbedtls
    OR NOT TARGET mbedx509
    OR NOT TARGET mbedcrypto
)
    set(MbedTLS_FOUND FALSE)

    set(MbedTLS_NOT_FOUND_MESSAGE
        "Mbed TLS targets were not found. "
        "Make sure FetchContent_MakeAvailable(mbedtls) "
        "was called before finding MbedTLS."
    )

    return()
endif()

# Compatibility targets

# TLS
if(NOT TARGET MbedTLS::MbedTLS)
    add_library(MbedTLS::MbedTLS INTERFACE IMPORTED GLOBAL)

    set_property(
        TARGET MbedTLS::MbedTLS
        PROPERTY INTERFACE_LINK_LIBRARIES
        mbedtls
    )
endif()

# Crypto
if(NOT TARGET MbedTLS::MbedCrypto)
    add_library(MbedTLS::MbedCrypto INTERFACE IMPORTED GLOBAL)

    set_property(
        TARGET MbedTLS::MbedCrypto
        PROPERTY INTERFACE_LINK_LIBRARIES
        mbedcrypto
    )
endif()

# X509
if(NOT TARGET MbedTLS::MbedX509)
    add_library(MbedTLS::MbedX509 INTERFACE IMPORTED GLOBAL)

    set_property(
        TARGET MbedTLS::MbedX509
        PROPERTY INTERFACE_LINK_LIBRARIES
        mbedx509
    )
endif()

# Package information

set(MbedTLS_VERSION "3.6.7")

set(MbedTLS_INCLUDE_DIR
    "${mbedtls_SOURCE_DIR}/include"
)

set(MbedTLS_INCLUDE_DIRS
    "${mbedtls_SOURCE_DIR}/include"
)

set(MBEDTLS_INCLUDE_DIRS
    "${mbedtls_SOURCE_DIR}/include"
)

set(MbedTLS_LIBRARY mbedtls)
set(MbedCrypto_LIBRARY mbedcrypto)
set(MbedX509_LIBRARY mbedx509)

set(MbedTLS_LIBRARIES
    mbedtls
    mbedx509
    mbedcrypto
)

set(MBEDTLS_LIBRARIES
    mbedcrypto
)

# Validate package

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    MbedTLS
    REQUIRED_VARS
        MbedTLS_INCLUDE_DIR
        MbedTLS_LIBRARY
        MbedCrypto_LIBRARY
        MbedX509_LIBRARY
    VERSION_VAR MbedTLS_VERSION
)

mark_as_advanced(MbedTLS_INCLUDE_DIR)
