/**
 * Fleet v2 HTTP API
 * HTTP-based API for Fleet Protocol v2 serving for communication between the External Server and the end users.
 *
 * The version of the OpenAPI document: 2.4.0
 * Contact: jiri.strouhal@bringauto.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.2.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * IHttpBody.h
 *
 * This is the interface for contents that can be sent to a remote HTTP server.
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_MODEL_IHttpBody_H_
#define ORG_OPENAPITOOLS_CLIENT_MODEL_IHttpBody_H_


#include <iostream>

namespace org {
namespace openapitools {
namespace client {
namespace model {

class  IHttpBody
{
public:
    virtual ~IHttpBody() { }

    virtual void writeTo( std::ostream& stream ) = 0;
};

}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_MODEL_IHttpBody_H_ */
