<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>distances.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>distances_8hpp.html</filename>
    <class kind="struct">knncolle_hnsw::DistanceConfig</class>
    <class kind="class">knncolle_hnsw::ManhattanDistance</class>
    <class kind="class">knncolle_hnsw::SquaredEuclideanDistance</class>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="file">
    <name>knncolle_hnsw.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>knncolle__hnsw_8hpp.html</filename>
    <includes id="load__hnsw__prebuilt_8hpp" name="load_hnsw_prebuilt.hpp" local="yes" import="no" module="no" objc="no">load_hnsw_prebuilt.hpp</includes>
    <includes id="distances_8hpp" name="distances.hpp" local="yes" import="no" module="no" objc="no">distances.hpp</includes>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="file">
    <name>load_hnsw_prebuilt.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>load__hnsw__prebuilt_8hpp.html</filename>
    <class kind="struct">knncolle_hnsw::HnswPrebuiltTypes</class>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::DistanceConfig</name>
    <filename>structknncolle__hnsw_1_1DistanceConfig.html</filename>
    <templarg>typename Distance_</templarg>
    <templarg>typename HnswData_</templarg>
    <member kind="variable">
      <type>std::function&lt; hnswlib::SpaceInterface&lt; HnswData_ &gt; *(std::size_t)&gt;</type>
      <name>create</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceConfig.html</anchorfile>
      <anchor>a631f9820cbc2e7fbbeab10a78019d483</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DistanceNormalizeMethod</type>
      <name>normalize_method</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceConfig.html</anchorfile>
      <anchor>a4aaf38f2e2c2dbe37c237456fa958280</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; Distance_(Distance_)&gt;</type>
      <name>custom_normalize</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceConfig.html</anchorfile>
      <anchor>a8304619fe87fcb3fdaa0d6e109ec1a97</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswBuilder</name>
    <filename>classknncolle__hnsw_1_1HnswBuilder.html</filename>
    <templarg>typename Index_</templarg>
    <templarg>typename Data_</templarg>
    <templarg>typename Distance_</templarg>
    <templarg>class Matrix_</templarg>
    <templarg>typename HnswData_</templarg>
    <base>knncolle::Builder&lt; typename Index_, typename Data_, typename Distance_, class Matrix_ &gt;</base>
    <member kind="function">
      <type></type>
      <name>HnswBuilder</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>ab5e4afeb34e906a90a335303cb4e6909</anchor>
      <arglist>(DistanceConfig&lt; Distance_, HnswData_ &gt; distance_config, HnswOptions options)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>HnswBuilder</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a97efed8df629c04f9d629b3642961a97</anchor>
      <arglist>(DistanceConfig&lt; Distance_, HnswData_ &gt; distance_config)</arglist>
    </member>
    <member kind="function">
      <type>HnswOptions &amp;</type>
      <name>get_options</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>af1657231b9f65ac42b06b35fc348269d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>build_known_raw</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>ad26bd41f4c63a16511fd3b231ffb41b5</anchor>
      <arglist>(const Matrix_ &amp;data) const</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>build_known_unique</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>ae79eae5cc0c86c99e7ac32433f4f9d7e</anchor>
      <arglist>(const Matrix_ &amp;data) const</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>build_known_shared</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a6c895af22e4e8e94eb745905f4ca7696</anchor>
      <arglist>(const Matrix_ &amp;data) const</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::HnswOptions</name>
    <filename>structknncolle__hnsw_1_1HnswOptions.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>num_links</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>a53d97ffff91e897c5612aefe23b17d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ef_construction</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>a772b371108bdacc6a79bdb176d071d4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ef_search</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>af1492d2ff195c4b59aa3d179efbcd394</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::HnswPrebuiltTypes</name>
    <filename>structknncolle__hnsw_1_1HnswPrebuiltTypes.html</filename>
    <member kind="variable">
      <type>knncolle::NumericType</type>
      <name>data</name>
      <anchorfile>structknncolle__hnsw_1_1HnswPrebuiltTypes.html</anchorfile>
      <anchor>a2e774092081ba86c4a178c49db3cf1ad</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::ManhattanDistance</name>
    <filename>classknncolle__hnsw_1_1ManhattanDistance.html</filename>
    <templarg>typename HnswData_</templarg>
    <member kind="function">
      <type></type>
      <name>ManhattanDistance</name>
      <anchorfile>classknncolle__hnsw_1_1ManhattanDistance.html</anchorfile>
      <anchor>ae6006a4cedae0ba35fb2481de5a289bf</anchor>
      <arglist>(std::size_t dim)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::SquaredEuclideanDistance</name>
    <filename>classknncolle__hnsw_1_1SquaredEuclideanDistance.html</filename>
    <templarg>typename HnswData_</templarg>
    <member kind="function">
      <type></type>
      <name>SquaredEuclideanDistance</name>
      <anchorfile>classknncolle__hnsw_1_1SquaredEuclideanDistance.html</anchorfile>
      <anchor>a11b2b4962b2a2746e624f057e10360ce</anchor>
      <arglist>(std::size_t dim)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>knncolle_hnsw</name>
    <filename>namespaceknncolle__hnsw.html</filename>
    <class kind="struct">knncolle_hnsw::DistanceConfig</class>
    <class kind="class">knncolle_hnsw::HnswBuilder</class>
    <class kind="struct">knncolle_hnsw::HnswOptions</class>
    <class kind="struct">knncolle_hnsw::HnswPrebuiltTypes</class>
    <class kind="class">knncolle_hnsw::ManhattanDistance</class>
    <class kind="class">knncolle_hnsw::SquaredEuclideanDistance</class>
    <member kind="enumeration">
      <type></type>
      <name>DistanceNormalizeMethod</name>
      <anchorfile>namespaceknncolle__hnsw.html</anchorfile>
      <anchor>a8040b86dbf364125600c3ffa0b17957d</anchor>
      <arglist></arglist>
      <enumvalue file="namespaceknncolle__hnsw.html" anchor="a8040b86dbf364125600c3ffa0b17957da36875f2500a09ee35d0bb7eb8c0b91b0">SQRT</enumvalue>
      <enumvalue file="namespaceknncolle__hnsw.html" anchor="a8040b86dbf364125600c3ffa0b17957da72baef04098f035e8a320b03ad197818">CUSTOM</enumvalue>
      <enumvalue file="namespaceknncolle__hnsw.html" anchor="a8040b86dbf364125600c3ffa0b17957dab50339a10e1de285ac99d4c3990b8693">NONE</enumvalue>
    </member>
    <member kind="function">
      <type>DistanceConfig&lt; Distance_, HnswData_ &gt;</type>
      <name>configure_euclidean_distance</name>
      <anchorfile>namespaceknncolle__hnsw.html</anchorfile>
      <anchor>a939b7a0484ad0abb8ad2771e22b2f763</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>DistanceConfig&lt; Distance_, HnswData_ &gt;</type>
      <name>configure_manhattan_distance</name>
      <anchorfile>namespaceknncolle__hnsw.html</anchorfile>
      <anchor>ac959f593dc491c48398c92d0aa56e579</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>HnswPrebuiltTypes</type>
      <name>load_hnsw_prebuilt_types</name>
      <anchorfile>namespaceknncolle__hnsw.html</anchorfile>
      <anchor>ac7dba32a21f28000a286bf843dd06e37</anchor>
      <arglist>(const std::string &amp;prefix)</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>load_hnsw_prebuilt</name>
      <anchorfile>namespaceknncolle__hnsw.html</anchorfile>
      <anchor>a346a21cc3b5f9146d23e5c9a060f3f4e</anchor>
      <arglist>(const std::string &amp;prefix)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>knncolle bindings for HNSW</title>
    <filename>index.html</filename>
    <docanchor file="index.html">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
