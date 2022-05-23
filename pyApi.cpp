#include "connectFour.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(module_name, handle)
{
    // py::class_<connectFour>(handle, "pyConnectFour")
    //     .def(py::init<>())
    //     .def("updateBoard", &connectFour::updateBoard);

    py::class_<connectFourClient>(handle, "PyConnectFourClient")
        .def(py::init<>())
        .def("join", &connectFourClient::join)
        .def("waitForOpponent", &connectFourClient::waitForOpponent)
        .def("handleNetworkEvent", &connectFourClient::handleNetworkEvent)
        .def("setColumn", &connectFourClient::setColumn)
        .def("getInPlace", &connectFourClient::getInPlace)
        .def("getPlayer", &connectFourClient::getPlayer)
        .def("sendAction", &connectFourClient::sendAction);
};