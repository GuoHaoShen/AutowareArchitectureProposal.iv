/*
 * Copyright 2020 Tier IV, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <rclcpp/rclcpp.hpp>

#include <scene_module/intersection/scene_intersection.h>
#include <spline_interpolation/spline_interpolation.h>

#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

namespace util
{
bool setVelocityFrom(
  const size_t idx, const double vel, autoware_planning_msgs::msg::PathWithLaneId * input);

bool splineInterpolate(
  const autoware_planning_msgs::msg::PathWithLaneId & input, const double interval,
  autoware_planning_msgs::msg::PathWithLaneId * output, const rclcpp::Logger logger);

int insertPoint(
  const geometry_msgs::msg::Pose & in_pose,
  autoware_planning_msgs::msg::PathWithLaneId * inout_path);

geometry_msgs::msg::Pose getAheadPose(
  const size_t start_idx, const double ahead_dist,
  const autoware_planning_msgs::msg::PathWithLaneId & path);

bool isAheadOf(const geometry_msgs::msg::Pose & target, const geometry_msgs::msg::Pose & origin);
bool hasLaneId(const autoware_planning_msgs::msg::PathPointWithLaneId & p, const int id);

/**
   * @brief get objective polygons for detection area
   */
bool getObjectivePolygons(
  lanelet::LaneletMapConstPtr lanelet_map_ptr, lanelet::routing::RoutingGraphPtr routing_graph_ptr,
  const int lane_id, const IntersectionModule::PlannerParam & planner_param,
  std::vector<lanelet::CompoundPolygon3d> * polygons, const rclcpp::Logger logger);

/**
   * @brief Generate a stop line and insert it into the path. If the stop line is defined in the map,
   * read it from the map; otherwise, generate a stop line at a position where it will not collide.
   * @param detection_areas used to generate stop line
   * @param path            ego-car lane
   * @param stop_line_idx   generated stop line index
   * @param pass_judge_line_idx  generated stop line index
   * @return false when generation failed
   */
bool generateStopLine(
  const int lane_id, const std::vector<lanelet::CompoundPolygon3d> detection_areas,
  const std::shared_ptr<const PlannerData> & planner_data,
  const IntersectionModule::PlannerParam & planner_param,
  autoware_planning_msgs::msg::PathWithLaneId * path, int * stop_line_idx,
  int * pass_judge_line_idx, int * first_idx_inside_lane, const rclcpp::Logger logger);

/**
   * @brief Calculate first path index that is in the polygon.
   * @param path     target path
   * @param polygons target polygon
   * @return path point index
   */
int getFirstPointInsidePolygons(
  const autoware_planning_msgs::msg::PathWithLaneId & path,
  const std::vector<lanelet::CompoundPolygon3d> & polygons);

/**
   * @brief Get stop point from map if exists
   * @param stop_pose stop point defined on map
   * @return true when the stop point is defined on map.
   */
bool getStopPoseFromMap(
  const int lane_id, geometry_msgs::msg::Point * stop_pose,
  const std::shared_ptr<const PlannerData> & planner_data);

}  // namespace util
