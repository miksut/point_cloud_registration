#include "knn_queries.hpp"


knn_queries::knn_queries( const point_cloud_t& pc_ )
  : _index( build_pts_list_index< dist_l2_t, point_cloud_t, 3 >( pc_ ) )
{}



int knn_queries::query_nn( const point_t& query_pt_,
                          float max_dist_ )
{
  flann::SearchParams query_params = flann::SearchParams( -1 );
    
  float* query_pt_ptr = const_cast< float* >( query_pt_.data() );
  flann::Matrix< float > query_pt( query_pt_ptr, 1, 3 );

  std::vector< std::vector< int > > indices;
  std::vector< std::vector< float > > distances;
  _index.knnSearch( query_pt, indices, distances, 1, query_params );

  int nn = -1;
  if( std::sqrt( distances[ 0 ][ 0 ] ) < max_dist_ )
  {
    nn = indices[ 0 ][ 0 ];
  }
  return nn;
}
