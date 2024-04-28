#ifndef LC_INCLUDE_OP_LAYER_H_
#define LC_INCLUDE_OP_LAYER_H_
#include <string>
#include <utility>
#include <vector>
#include "tensor/tensor.h"

namespace op {
enum class LayerType : uint8_t {
  kLayerUnknown = 0,
  kLayerLinear = 1,
  kLayerEncode = 2,
  kLayerEmbedding = 3,
};

class BaseLayer {
 public:
  explicit BaseLayer(LayerType layer_type, base::DataType data_type, std::string layer_name = "");

  base::DataType data_type() const;

  LayerType layer_type() const;

  virtual base::Status init() = 0;

  virtual base::Status forward() = 0;

  virtual void set_input(int32_t idx, const tensor::Tensor& input) = 0;

  virtual void set_output(int32_t idx, const tensor::Tensor& output) = 0;

  virtual tensor::Tensor get_input(int32_t idx) const = 0;

  virtual tensor::Tensor get_output(int32_t idx) const = 0;

  virtual void reset_input_size(size_t size) = 0;

  virtual void reset_output_size(size_t size) = 0;

 private:
  std::string layer_name_;
  base::DataType data_type_ = base::DataType::kDataTypeUnknown;
  LayerType layer_type_ = LayerType::kLayerUnknown;
};

class LayerFp32 : public BaseLayer {
 public:
  explicit LayerFp32(LayerType layer_type, std::string layer_name = "");

  base::Status init() override;

  base::Status forward() override;

  void set_input(int32_t idx, const tensor::Tensor& input) override;

  void set_output(int32_t idx, const tensor::Tensor& output) override;

  tensor::Tensor get_input(int32_t idx) const override;

  tensor::Tensor get_output(int32_t idx) const override;

  void reset_input_size(size_t size) override;

  void reset_output_size(size_t size) override;

 private:
  std::vector<tensor::Tensor> inputs_;
  std::vector<tensor::Tensor> outputs_;
};

class LayerFp32Param : public LayerFp32 {
 public:
  explicit LayerFp32Param(LayerType layer_type, std::string layer_name = "");

  void set_weight(int32_t idx, const tensor::Tensor& weight);

  tensor::Tensor get_weight(int32_t idx) const;

  void set_weight(int32_t idx, const std::vector<int32_t>& dims, const float* weight_ptr);

  void reset_weight_size(size_t size);

 private:
  std::vector<tensor::Tensor> weights_;
  std::vector<tensor::Tensor> inputs_;
  std::vector<tensor::Tensor> outputs_;
};
}  // namespace op
#endif  // LC_INCLUDE_OP_LAYER_H_
