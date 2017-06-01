const path = require('path');
const ExtractTextPlugin = require('extract-text-webpack-plugin');
const HtmlWebpackPlugin = require("html-webpack-plugin");
module.exports = {
  entry: {
    app: [
      "babel-polyfill",
      "./lib/js/src/app",
      "./src/styles/index"
    ],
  },
  output: {
    path: path.join(__dirname, "dist"),
    filename: 'app.js',
  },
  resolve: {
    extensions: [".js", ".jsx", ".scss", ".css"],
  },
  module: {
    loaders: [
      {
        test: /\.css$/,
        loader: ExtractTextPlugin.extract("css-loader")
      },
      {
        test: /\.scss$/,
        loaders: ExtractTextPlugin.extract('css-loader!sass-loader'),
      }
    ],
  },
  plugins: [
    new ExtractTextPlugin({
        filename: 'styles.css',
        allChunks: true
    }),
    new HtmlWebpackPlugin({
      inject: false,
      template: './src/index.html'
    }),
  ]
};
