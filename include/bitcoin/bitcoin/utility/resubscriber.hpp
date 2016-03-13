/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef  LIBBITCOIN_RESUBSCRIBER_HPP
#define  LIBBITCOIN_RESUBSCRIBER_HPP

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/dispatcher.hpp>
#include <bitcoin/bitcoin/utility/enable_shared_from_base.hpp>
#include <bitcoin/bitcoin/utility/thread.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>
////#include <bitcoin/bitcoin/utility/track.hpp>

namespace libbitcoin {

template <typename... Args>
class resubscriber
  : public enable_shared_from_base<resubscriber<Args...>>/*,
    track<resubscriber<Args...>>*/
{
public:
    typedef std::function<bool (Args...)> handler;
    typedef std::shared_ptr<resubscriber<Args...>> ptr;

    resubscriber(threadpool& pool, const std::string& class_name);
    ~resubscriber();

    /// Call start to enable new subscriptions.
    void start();

    /// Call stop to prevent new subscriptions.
    void stop();

    /// Return true from the handler to resubscribe.
    void subscribe(handler notifier, Args... stopped_args);

    /// Call to invoke all handlers, which clears subscription if false.
    void relay(Args... args);

    /// Invoke all handlers in order on the current thread.
    void do_relay(Args... args);

private:
    typedef std::vector<handler> list;

    bool stopped_;
    dispatcher dispatch_;
    list subscriptions_;
    mutable upgrade_mutex mutex_;
};

} // namespace libbitcoin

#include <bitcoin/bitcoin/impl/utility/resubscriber.ipp>

#endif
