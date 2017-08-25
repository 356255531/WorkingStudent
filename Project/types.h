#ifndef TYPES_H
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
/**
* Limit the CPU concurrency (Thread number) to pool_size and dynamically manage the thread
* @param pool_size Value of maximum number of thread
* @see The usage of the thread pool:
* @code
*    void print_num(int num) {
*        std::cout<<num<<std::endl;
*    }
*    int main() {
*        n_threads = 4;
*        ThreadPool thread_pool(n_threads);
*        for(auto i = start_num; i < end_num; i++){
*            int im_x = 0;
*            thread_pool.enqueue (boost::bind(&print_num, i));
*        }
*    }
* @endcode
*/
struct ThreadPool {
    typedef boost::scoped_ptr<boost::asio::io_service::work> boost_asio_worker;

    ThreadPool(size_t pool_size) :m_service(), m_working(new boost::asio::io_service::work(m_service)) {
        while(pool_size--)
        {
            m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &(this->m_service)));
        }
    }

    template<class F>
        void enqueue(F f){m_service.post(f);}

    ~ThreadPool() {
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