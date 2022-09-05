const path = require('path');
const fs = require("fs");
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyPlugin = require("copy-webpack-plugin");

let mode = "development";

module.exports = {
  entry: './src/index.ts',
  output: {
    filename: 'bundle.js',
    path: path.resolve(__dirname, 'dist'),
  },
  resolve: {
    extensions: [ '.tsx', '.ts', '.js' ],
    alias: {
      fs: false,
      path: false,
      crypto: false,
    }
  },
  mode: mode,
  devtool: 'inline-source-map',
  devServer: {
    headers: {
      "Cross-Origin-Opener-Policy": "same-origin",
      "Cross-Origin-Embedder-Policy": "require-corp"
    },
    static: {
      directory: path.join(__dirname, 'wasm'),
    },
    compress: true,
    port: 9000,
  },
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        use: 'ts-loader',
        exclude: /node_modules/,
      },
    ],
  },
  plugins: [
    // new HtmlWebpackPlugin({
    //   templateContent: fs.readFileSync(path.resolve(__dirname, "wasm/index.html")).toString()
    // }),
    new CopyPlugin({
      patterns: [
        { from: "wasm", to: "" },
      ],
    }),
  ],
};