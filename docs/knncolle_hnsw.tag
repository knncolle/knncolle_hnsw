<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="file">
    <name>distances.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>distances_8hpp.html</filename>
    <class kind="struct">knncolle_hnsw::DistanceOptions</class>
    <class kind="class">knncolle_hnsw::ManhattanDistance</class>
    <class kind="class">knncolle_hnsw::SquaredEuclideanDistance</class>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="file">
    <name>knncolle_hnsw.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>knncolle__hnsw_8hpp.html</filename>
    <includes id="distances_8hpp" name="distances.hpp" local="yes" import="no" module="no" objc="no">distances.hpp</includes>
    <class kind="struct">knncolle_hnsw::HnswOptions</class>
    <class kind="class">knncolle_hnsw::HnswSearcher</class>
    <class kind="class">knncolle_hnsw::HnswPrebuilt</class>
    <class kind="class">knncolle_hnsw::HnswBuilder</class>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::DistanceOptions</name>
    <filename>structknncolle__hnsw_1_1DistanceOptions.html</filename>
    <templarg>typename Dim_</templarg>
    <templarg>typename InternalData_</templarg>
    <member kind="variable">
      <type>std::function&lt; hnswlib::SpaceInterface&lt; InternalData_ &gt; *(Dim_)&gt;</type>
      <name>create</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceOptions.html</anchorfile>
      <anchor>a450d9f52efe81407caaac5718af983e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; InternalData_(InternalData_)&gt;</type>
      <name>normalize</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceOptions.html</anchorfile>
      <anchor>a66809442cd1f399975c0a330aecf34d4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswBuilder</name>
    <filename>classknncolle__hnsw_1_1HnswBuilder.html</filename>
    <templarg>class Matrix_</templarg>
    <templarg>typename Float_</templarg>
    <templarg>typename InternalData_</templarg>
    <base>knncolle::Builder</base>
    <member kind="typedef">
      <type>HnswOptions&lt; typename Matrix_::dimension_type, InternalData_ &gt;</type>
      <name>Options</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a102a68782bb838cdeb435d263c560734</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>HnswPrebuilt&lt; typename Matrix_::dimension_type, typename Matrix_::index_type, Float_, InternalData_ &gt;</type>
      <name>Prebuilt</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a6587c0c801ddbcb48638cacd5e31d134</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>HnswBuilder</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a885e29970c6224827af556dc442149e1</anchor>
      <arglist>(Options options)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>HnswBuilder</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a98b348b3af8de5e5dcbd9cf89a936cb7</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>Options &amp;</type>
      <name>get_options</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a63a2825cec1d3ca1a1addbca31dbfa6c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::HnswOptions</name>
    <filename>structknncolle__hnsw_1_1HnswOptions.html</filename>
    <templarg>typename Dim_</templarg>
    <templarg>typename InternalData_</templarg>
    <member kind="variable">
      <type>int</type>
      <name>num_links</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>adb0ef8a4107a0c5b6b8dd2677fa33b12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ef_construction</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>ac66e95af2ecc3d5e79ca5c373a575315</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ef_search</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>ad933bb74db25a15bd71324261a31c878</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DistanceOptions&lt; Dim_, InternalData_ &gt;</type>
      <name>distance_options</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>ab227b6637e409e1cab8d26496c7061d1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswPrebuilt</name>
    <filename>classknncolle__hnsw_1_1HnswPrebuilt.html</filename>
    <templarg>typename Dim_</templarg>
    <templarg>typename Index_</templarg>
    <templarg>typename Float_</templarg>
    <templarg>typename InternalData_</templarg>
    <base>Prebuilt&lt; Dim_, Index_, Float_ &gt;</base>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswSearcher</name>
    <filename>classknncolle__hnsw_1_1HnswSearcher.html</filename>
    <templarg>typename Dim_</templarg>
    <templarg>typename Index_</templarg>
    <templarg>typename Float_</templarg>
    <templarg>typename InternalData_</templarg>
    <base>Searcher&lt; Index_, Float_ &gt;</base>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::ManhattanDistance</name>
    <filename>classknncolle__hnsw_1_1ManhattanDistance.html</filename>
    <templarg>typename InternalData_</templarg>
    <member kind="function">
      <type></type>
      <name>ManhattanDistance</name>
      <anchorfile>classknncolle__hnsw_1_1ManhattanDistance.html</anchorfile>
      <anchor>a084c1f05a408864ed1e53c12d45a14d3</anchor>
      <arglist>(size_t dim)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::SquaredEuclideanDistance</name>
    <filename>classknncolle__hnsw_1_1SquaredEuclideanDistance.html</filename>
    <templarg>typename InternalData_</templarg>
    <member kind="function">
      <type></type>
      <name>SquaredEuclideanDistance</name>
      <anchorfile>classknncolle__hnsw_1_1SquaredEuclideanDistance.html</anchorfile>
      <anchor>aa589f16d412546b032dfd7de97aa4e59</anchor>
      <arglist>(size_t dim)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>knncolle_hnsw</name>
    <filename>namespaceknncolle__hnsw.html</filename>
    <class kind="struct">knncolle_hnsw::DistanceOptions</class>
    <class kind="class">knncolle_hnsw::HnswBuilder</class>
    <class kind="struct">knncolle_hnsw::HnswOptions</class>
    <class kind="class">knncolle_hnsw::HnswPrebuilt</class>
    <class kind="class">knncolle_hnsw::HnswSearcher</class>
    <class kind="class">knncolle_hnsw::ManhattanDistance</class>
    <class kind="class">knncolle_hnsw::SquaredEuclideanDistance</class>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>knncolle bindings for Hnsw</title>
    <filename>index.html</filename>
    <docanchor file="index.html">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
