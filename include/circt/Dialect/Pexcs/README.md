# PEXCS Dialect

PEXCS (Parallel Extensions for Computational Structures) 是一个MLIR方言，用于表达和优化计算结构，特别是在硬件设计和优化中常用的模式。

## 主要特点

- **数据流操作**: 提供map, reduce, filter等高阶函数模式，便于表达并行计算
- **类型系统**: 支持UI2, UI4等特化类型，简化位宽表示
- **可组合性**: 设计为与其他CIRCT方言良好集成

## 用法示例

```mlir
// 映射操作示例
%result = pexcs.map(%input) {mapper = @square_fn} : tensor<10xi32> -> tensor<10xi32>

// 规约操作示例
%sum = pexcs.reduce(%input, %init_val) {reducer = @add_fn} : (tensor<10xi32>, i32) -> i32

// 过滤操作示例
%filtered = pexcs.filter(%input) {predicate = @is_positive} : tensor<10xi32> -> tensor<?xi32>
```

## 扩展

PEXCS方言计划将来添加更多功能，包括：

- 优化转换
- 硬件映射规则
- 特定领域算法模式 