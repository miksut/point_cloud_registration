#pragma once

#include "common.h"


template< typename DIST_T, typename PTS_LIST_T, int DIMS >
flann::Index< DIST_T > build_pts_list_index( const PTS_LIST_T& pts_ )
{
  std::size_t n_pts = pts_.size();
  float* pts_ptr = new float[ n_pts * DIMS ];
  for( std::size_t p = 0; p < n_pts; p++ )
  {
    for( int c = 0; c < DIMS; c++ )
    {
      pts_ptr[ DIMS * p + c ] = pts_[ p ]( c );
    }
  }
  flann::Matrix< float > pts_matr( pts_ptr, n_pts, DIMS );
  flann::Index< DIST_T > index( pts_matr, flann::KDTreeIndexParams( 1 ) );
  index.buildIndex();
  return index;
}


class knn_queries
{
  public:
    knn_queries( const point_cloud_t& pc_ );

  private:
    index_l2_t _index;

  public:
    int query_nn( const point_t& query_pt_,
                  float max_dist_ = std::numeric_limits< float >::max() );
};
