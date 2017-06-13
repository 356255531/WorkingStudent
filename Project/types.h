#ifndef TYPES_H
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
// The definition of boost_asio based thread pool
struct gr_thread_pool {
    typedef boost::scoped_ptr<boost::asio::io_service::work> boost_asio_worker;

    gr_thread_pool(size_t pool_size) :m_service(), m_working(new boost::asio::io_service::work(m_service)) {
        while(pool_size--)
        {
            m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &(this->m_service)));
        }
    }

    template<class F>
        void enqueue(F f){m_service.post(f);}

    ~gr_thread_pool() {
        m_working.reset(); //allow run() to exit
        m_thread_group.join_all();
        m_service.stop();
    }

    private:
    boost::asio::io_service m_service; //< the io_service we are wrapping
    boost_asio_worker m_working;
    boost::thread_group m_thread_group; //< need to keep track of threads so we can join them
};
#endif