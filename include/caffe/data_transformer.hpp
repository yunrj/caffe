#ifndef CAFFE_DATA_TRANSFORMER_HPP
#define CAFFE_DATA_TRANSFORMER_HPP

#include "caffe/common.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/io.hpp"

namespace caffe {

/**
 * @brief Applies common transformations to the input data, such as
 * scaling, mirroring, substracting the image mean...
 */
template <typename Dtype>
class DataTransformer {
 public:
  explicit DataTransformer(const TransformationParameter& param)
    : param_(param) {
    phase_ = Caffe::phase();
      // check if we want to have mean
    if (param_.has_mean_file()) {
      const string& mean_file = param.mean_file();
      LOG(INFO) << "Loading mean file from" << mean_file;
      BlobProto blob_proto;
      ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);
      data_mean_.FromProto(blob_proto);
    }
  }
  virtual ~DataTransformer() {}

  void InitRand();

  /**
   * @brief Applies the transformation defined in the data layer's
   * transform_param block to the data.
   *
   * @param batch_item_id
   *    Datum position within the batch. This is used to compute the
   *    writing position in the top blob's data
   * @param datum
   *    Datum containing the data to be transformed.
   * @param transformed_data
   *    This is meant to be the top blob's data. The transformed data will be
   *    written at the appropriate place within the blob's data.
   */

  void Transform(const int batch_item_id, const Datum& datum,
                 Dtype* transformed_data);

 protected:
   /**
   * @brief Generates a random integer from Uniform({0, 1, ..., n-1}).
   * 
   * @param n
   *    The upperbound (exclusive) value of the random number.
   * @return 
   *    A uniformly random integer value from ({0, 1, ..., n-1}).
   */
  virtual int Rand(int n);

  void Transform(const int batch_item_id, const Datum& datum,
                 const Dtype* mean, Dtype* transformed_data);
  // Tranformation parameters
  TransformationParameter param_;


  shared_ptr<Caffe::RNG> rng_;
  Caffe::Phase phase_;
  Blob<Dtype> data_mean_;
};

}  // namespace caffe

#endif  // CAFFE_DATA_TRANSFORMER_HPP_

