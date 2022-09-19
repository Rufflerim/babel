//
// Created by gaetz on 04/10/2021.
//

#ifndef EVENTS_H
#define EVENTS_H

#include <functional>
#include <array>
#include <vector>
#include "Log.h"

using std::array;
using std::vector;

namespace engine {

    enum class EventCode {
        ApplicationQuit,
        WindowResized,

        MaxEventCode
    };

    constexpr u16 MAX_EVENT_CODE = 16384;

    using EventCallback = std::function<bool(engine::EventCode code, void *sender, void *listenerInstance)>;

    struct Subscription {
        void* listener { nullptr };
        EventCallback* callback { nullptr };

        bool operator==(const Subscription& other) const {
            return listener == other.listener && callback == other.callback;
        }
    };

    struct EventCodeEntry {
        vector<Subscription> subscriptions {};
    };

    class Events {
    public:
        /**
         * Subscribe to listen events with the provided code.
         * Events with duplicate listener/callback combo will not be subscribed
         * and cause this function to return false.
         * @param code Event code to listen for.
         * @param listener Listener instance reference.
         * @param onEvent Callback function to be invoked when the event code is fired.
         * @returns true when the event is successfully subscribed, false otherwise.
         */
        GAPI virtual bool subscribe(EventCode code, void *listener, EventCallback* onEvent) = 0;

        /**
         * Unsubscribe from to listening to events with the provided code.
         * If no subscription is found, this function to return false.
         * @param code Event code to listen for.
         * @param listener Listener instance reference.
         * @param onEvent Callback function to be unregistred from.
         * @returns true when the event is successfully unsubscribed, false otherwise.
         */
        GAPI virtual bool unsubscribe(EventCode code, void *listener, EventCallback* onEvent) = 0;

        /**
         * Fire an event to listeners of the given code. If an event handler returns true,
         * the event is considered handled and is not passed on to any more listeners.
         * @param code Code of the event to fire.
         * @param sender Reference to the sender.
         * @return True if handled, otherwise false.
         */
        GAPI virtual bool fire(EventCode code, void *sender) = 0;

        virtual ~Events() = default;
    };

    class NullEvents : public Events {
    private:
        void placeholderMessage(EventCode code, void *listener, EventCallback* onEvent) {
#ifdef GPLATFORM_WEB
            // Usage of all arguments to avoid warnings
            listener = 0;
            onEvent = 0;
            LOG(LogLevel::Warning) << "Usage of placeholder event service. Code: " << static_cast<i32>(code) << " Trash:" << listener << onEvent;
#else
            listener = nullptr;
            onEvent = nullptr;
            LOG(LogLevel::Warning) << "Usage of placeholder event service. Code: " << static_cast<i32>(code);
#endif
        }

        void placeholderMessage(EventCode code, void *sender) {
#ifdef GPLATFORM_WEB
            // Usage of all arguments to avoid warnings
            sender = 0;
            LOG(LogLevel::Warning) << "Usage of placeholder event service. Code: "
                << static_cast<i32>(code) << " Trash:" << sender;
#else
            sender = nullptr;
            LOG(LogLevel::Warning) << "Usage of placeholder event service. Code: "<< static_cast<i32>(code);
#endif
        }

    public:
        GAPI bool subscribe(EventCode code, void *listener, EventCallback* onEvent) override { placeholderMessage(code, listener, onEvent); return false; }
        GAPI bool unsubscribe(EventCode code, void *listener, EventCallback* onEvent) override { placeholderMessage(code, listener, onEvent); return false; }
        GAPI bool fire(EventCode code, void *sender) override { placeholderMessage(code, sender); return false; }
    };
}

#endif //EVENTS_H
