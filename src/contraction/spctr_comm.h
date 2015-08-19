/*Copyright (c) 2011, Edgar Solomonik, all rights reserved.*/

#ifndef __SPCTR_COMM_H__
#define __SPCTR_COMM_H__

#include "ctr_comm.h"

namespace CTF_int{

  class spctr : public ctr {
    public: 
      bool      is_sparse_A;
      int64_t   nnz_A;
      int       nvirt_A;
      int64_t * nnz_blk_A;
      bool      is_sparse_B;
      int64_t   nnz_B;
      int       nvirt_B;
      int64_t * nnz_blk_B;
      bool      is_sparse_C;
      int64_t   nnz_C;
      int       nvirt_C;
      int64_t * nnz_blk_C;
      int64_t   new_nnz_C;
      char *    new_C;

      ~spctr();
      spctr(spctr * other);
      virtual spctr * clone() { return NULL; }
      spctr(contraction const * c);
      virtual void set_nnz_blk_A(int64_t const * nnbA){
        if (nnbA != NULL) memcpy(nnz_blk_A, nnbA, nvirt_A*sizeof(int64_t));
      }
      virtual void set_nnz_blk_B(int64_t const * nnbB){
        if (nnbB != NULL) memcpy(nnz_blk_B, nnbB, nvirt_B*sizeof(int64_t));
      }
  };

  class seq_tsr_spctr : public spctr {
    public:
      char const * alpha;
      int         order_A;
      int *       edge_len_A;
      int const * idx_map_A;
      int *       sym_A;

      int         order_B;
      int *       edge_len_B;
      int const * idx_map_B;
      int *       sym_B;
      
      int         order_C;
      int *       edge_len_C;
      int const * idx_map_C;
      int *       sym_C;

      int is_inner;
      iparam inner_params;
      
      int is_custom;
      bivar_function const * func; // custom_params;
      

      /**
       * \brief wraps user sequential function signature
       */
      void run();
      void print();
      int64_t mem_fp();
      spctr * clone();
      void set_nnz_blk_A(int64_t const * nnbA){
        spctr::set_nnz_blk_A(nnbA);
      }
      void set_nnz_blk_B(int64_t const * nnbB){
        spctr::set_nnz_blk_B(nnbB);
      }
      /**
       * \brief returns the execution time the local part this kernel is estimated to take
       * \return time in sec
       */
      double est_time_fp(int nlyr);
      /**
       * \brief returns the execution time this kernel and its recursive calls are estimated to take
       * \return time in sec
       */
      double est_time_rec(int nlyr);

      /**
       * \brief copies ctr object
       * \param[in] other object to copy
       */
      seq_tsr_spctr(spctr * other);
      ~seq_tsr_spctr(){ 
        CTF_int::cdealloc(edge_len_A), CTF_int::cdealloc(edge_len_B), CTF_int::cdealloc(edge_len_C), 
        CTF_int::cdealloc(sym_A), CTF_int::cdealloc(sym_B), CTF_int::cdealloc(sym_C); 
      }

      seq_tsr_spctr(contraction const * s,
                    bool                is_inner,
                    iparam const *      inner_params,
                    int *               virt_blk_len_A,
                    int *               virt_blk_len_B,
                    int *               virt_blk_len_C,
                    int64_t             vrt_sz_C);

  };

}

#endif
