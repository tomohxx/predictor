#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "predictor.hpp"
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)
namespace py = pybind11;

PYBIND11_MODULE(predictor, m)
{
  py::class_<pred::Value>(m, "Value")
      .def(py::init<>())
      .def_readonly("all", &pred::Value::all)
      .def_readonly("states", &pred::Value::states)
      .def("props", &pred::Value::props)
      .def("props100", &pred::Value::props100);

  py::class_<pred::Predictor>(m, "Predictor")
      .def(py::init<>())
      .def("initialize", &pred::Predictor::initialize)
      .def("__call__", &pred::Predictor::operator());

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
