# Copyright 2013-2024 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack.package import *


class Occa(CMakePackage):
    """OCCA: Portable approach for parallel architectures."""

    homepage = "https://libocca.org"
    git      = "https://github.com/dylanjude/occa.git"
    url      = "https://github.com/dylanjude/occa/archive/refs/tags/v2.0.0.tar.gz"

    maintainers("dylanjude")
    license("MIT")

    version("main", branch="main")
    version("2.0.0", sha256="f2521901fed5d199193d54c7db4186479a974bdae92ac97779c47fa2bb68badd")
    version("1.6.0", sha256="b863a24171000097121aff5c43dadf22416c143824598a51c653689fd917794f")
    version("1.5.0", sha256="b939f826f3e970b45aa77089568995399355fcdad3bc787b60cf73eb4962b0b7")
    version("1.4.0", sha256="5995288615f45dd2cf2f3e13b9e04c3b89edde1e014d36c6da014c11b9adb4a7")

    variant("cuda",   default=True,  description="Activates support for CUDA")
    variant("hip",    default=False, description="Activates support for HIP")
    variant("openmp", default=True,  description="Activates support for OpenMP")
    variant("opencl", default=False, description="Activates support for OpenCL")
    variant("dpcpp",  default=False, description="Activates support for DPCPP")

    depends_on("c", type="build")
    depends_on("cxx", type="build")

    depends_on("cuda", when="+cuda")
    depends_on("hip", when="+hip")

    depends_on("cmake", type="build")

    def cmake_args(self):
        args = [
            self.define("CMAKE_BUILD_TYPE", "Release"),
            self.define_from_variant("OCCA_ENABLE_CUDA", "cuda"),
            self.define_from_variant("OCCA_ENABLE_HIP", "hip"),
            self.define_from_variant("OCCA_ENABLE_OPENMP", "openmp"),
            self.define_from_variant("OCCA_ENABLE_OPENCL", "opencl"),
            self.define_from_variant("OCCA_ENABLE_DPCPP", "dpcpp"),
        ]
        return args

    def setup_dependent_build_environment(self, env, dependent_spec):
        env.set("OCCA_INSTALL_DIR", self.prefix)
        env.set("OCCA_DIR", self.prefix)
        env.set("OCCA_HOME", self.prefix)

    def setup_run_environment(self, env):
        env.set("OCCA_INSTALL_DIR", self.prefix)
        env.set("OCCA_DIR", self.prefix)
        env.set("OCCA_HOME", self.prefix)
