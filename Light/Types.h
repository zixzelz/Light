//
//  Types.h
//  Light
//
//  Created by Ruslan Maslouski on 1/4/18.
//  Copyright © 2018 Ruslan Maslouski. All rights reserved.
//

#ifndef Types_h
#define Types_h

    struct _DNSSDTXTDeviceState {
        enum Local: char {Setup = 0, Configured = 1};
    };
    typedef _DNSSDTXTDeviceState::Local DNSSDTXTDeviceState;

#endif /* Types_h */
