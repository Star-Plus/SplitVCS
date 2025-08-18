#include <pybind11/pybind11.h>

#include <SplitInterface.h>

namespace py = pybind11;

PYBIND11_MODULE(pyscm, m) {

    m.doc() = "Python bindings for SCM (Source Code Management)";

    m.def("version", []() {
        return "SCM Version 1.0.0";
    });

    m.def("init", [](const str& repoPath) {
        Split::SplitInterface::Init(repoPath);
        return "Repository initialized at " + repoPath;
    });

    m.def("add", [](const str& rootPath, const str& filePath) {
        Split::SplitInterface::Add(rootPath, filePath);
    });

    m.def("commit", [](const str& rootPath, const str& message, const str& author = "default") {
        Split::SplitInterface::Commit(rootPath, message, author);
    });

    m.def("checkout", [](const str& rootPath, const str& commitId) {
        Split::SplitInterface::Checkout(rootPath, commitId);
    });

}